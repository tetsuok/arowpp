// Copyright (c) 2010-2012, Tetsuo Kiso
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above
//    copyright notice, this list of conditions and the
//    following disclaimer.
//
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the
//    following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
//  * Neither the name of Tetsuo Kiso nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <fstream>
#include "arowpp.h"
#include "common.h"
#include "feature.h"
#include "scoped_ptr.h"
#include "tokenizer.h"

namespace arowpp {

class Features;
class Result;

class BinaryClassifierImpl : public BinaryClassifier {
 public:
  BinaryClassifierImpl()
      : num_iter_(0),
        num_feature_(0),
        num_example_(0),
        num_update_(0),
        r_(0.1),
        is_shuffled_(false),
        features_(NULL) {}

  virtual ~BinaryClassifierImpl() {}

  virtual bool Train(const char* filename);
  virtual bool Open(const char* filename);
  virtual bool Save(const char* filename);
  virtual bool Load(const char* filename);

  virtual bool Classify(const char* line, Result* result) {
    short label = 0;                      // true label
    fv_t fv;
    size_t dummy = 0;                     // dummy id
    CHECK_FALSE(Tokenizer::Tokenize(line, &fv, &label, &dummy))
        << "cannot classify ";
    CHECK_FALSE(result->Add(label, Predict(fv)));

    return true;
  }

  virtual int get_num_iter() const { return num_iter_; }
  virtual unsigned int get_num_feature() const { return num_feature_; }
  virtual size_t get_num_example() const { return num_example_; }
  virtual size_t get_num_update() const { return num_update_; }
  virtual double get_r() const { return r_; }
  virtual bool is_shuffled() const { return is_shuffled_; }

  virtual void set_num_feature(unsigned int num_feature) { num_feature_ = num_feature; }
  virtual void set_num_iter(int num_iter) { num_iter_ = num_iter; }
  virtual void set_r(double r) { r_ = r; }
  virtual void set_shuffle(bool flag) { is_shuffled_ = flag; }

  virtual const char* what() { return what_.str(); }

 private:
  // Updates mean and confindence parameters.
  void Update(fv_t &fv, short label);

  // Makes a prediction from sign of margin.
  // Returns the predicted label.
  int Predict(fv_t &fv) {
    ResizeWeight(fv);
    const double m = GetMargin(fv);
    return m > 0.0 ? 1 : -1;
  }

  double GetMargin(fv_t &fv) const {
    double res = 0.0;
    for (fv_t::iterator it = fv.begin(); it != fv.end(); ++it) {
      res += mean_[it->first] * it->second;
    }
    return res;
  }

  double GetConfidence(fv_t &fv) const {
    double res = 0.0;
    for (fv_t::iterator it = fv.begin(); it != fv.end(); ++it) {
      res += cov_[it->first] * it->second * it->second;
    }
    return res;
  }

  // Resizes mean and covariance parameters.
  void ResizeWeight(fv_t &fv);

  // Allocates mean vector and covariance matrix.
  void Allocate();

  // We disable the default assignment operator and copy constructor.
  BinaryClassifierImpl(const BinaryClassifierImpl&);
  const BinaryClassifierImpl& operator=(const BinaryClassifierImpl&);

  int num_iter_;                        // Number of iteration
  unsigned int num_feature_;            // Number of features
  std::size_t num_example_;             // Number of examples
  std::size_t num_update_; // Number of update of the confidence parameters
  double r_;                            // hyperparameter
  bool is_shuffled_;                    // flag judgement
  scoped_ptr<Features> features_;       // Feature vector
  std::vector<float> mean_;            // mean parameters
  std::vector<float> cov_; // covariance matrix with diagonal elements
  whatlog what_;                        // store error log
};

bool BinaryClassifierImpl::Train(const char *filename) {
  CHECK_FALSE(Open(filename)) << "Cannot read " << filename;

  if (is_shuffled_) {
    features_->Shuffle();
  }

  for (int i = 0; i < num_iter_; ++i) {
    for (std::size_t j = 0; j < features_->size(); ++j) {
      std::pair<fv_t, short> instance = features_->get_instance(j);
      Update(instance.first, instance.second);
    }
  }
  return true;
}

bool BinaryClassifierImpl::Open(const char *filename) {
  features_.reset(new Features);
  CHECK_FALSE(features_->Open(filename)) << "no such file or directory: "
                                         << filename << " "
                                         << features_->what();
  num_feature_ = features_->maxid();
  Allocate();
  return true;
}

bool BinaryClassifierImpl::Save(const char *filename) {
  std::ofstream bofs;
  bofs.open(filename, std::ios::out | std::ios::binary);
  CHECK_FALSE(bofs) << "no such file or directory: " << filename;

  bofs.write(reinterpret_cast<char *>(&num_feature_), sizeof(num_feature_));
  bofs.write(reinterpret_cast<char *>(&num_example_), sizeof(num_example_));
  bofs.write(reinterpret_cast<char *>(&num_update_), sizeof(num_update_));
  bofs.write(reinterpret_cast<char *>(&r_), sizeof(r_));
  bofs.write(reinterpret_cast<char *>(&is_shuffled_), sizeof(is_shuffled_));

  // mean
  bofs.write(reinterpret_cast<char *>(&mean_[0]), mean_.size() * sizeof(float));

  // covariance
  bofs.write(reinterpret_cast<char *>(&cov_[0]), cov_.size() * sizeof(float));

  bofs.close();

  CHECK_FALSE(num_example_ != 0) << "Could not train examples";

  std::cout << "Number of features: " << num_feature_ << "\n"
            << "Number of examples: "
            << static_cast<double>(num_example_) / num_iter_ << "\n"
            << "Number of updates:  " << num_update_ << std::endl;

  return true;
}

bool BinaryClassifierImpl::Load(const char *filename) {
  std::ifstream bifs(filename, std::ios::in | std::ios::binary);
  CHECK_FALSE(bifs) << "Cannot load: no such file or directory: " << filename;

  // Get number of feature, number of examples and number of update
  bifs.read(reinterpret_cast<char *>(&num_feature_), sizeof(num_feature_));
  bifs.read(reinterpret_cast<char *>(&num_example_), sizeof(num_example_));
  bifs.read(reinterpret_cast<char *>(&num_update_), sizeof(num_update_));
  bifs.read(reinterpret_cast<char *>(&r_), sizeof(r_));
  bifs.read(reinterpret_cast<char *>(&is_shuffled_), sizeof(is_shuffled_));

  bool flag = true;
  if      (!num_feature_) { flag = false; }
  else if (!num_example_) { flag = false; }
  else if (!num_update_)  { flag = false; }
  else if (!r_)           { flag = false; }

  if (!flag) {
    std::cerr << "Error: load parameter" << std::endl;
    bifs.close();
    return false;
  }

  // Initialize
  Allocate();

  bifs.read(reinterpret_cast<char *>(&mean_[0]), mean_.size() * sizeof(float));
  bifs.read(reinterpret_cast<char *>(&cov_[0]), cov_.size() * sizeof(float));

  bifs.close();

  return true;
}

void BinaryClassifierImpl::Update(fv_t &fv, short label) {
  ++num_example_;
  const double m = GetMargin(fv);
  // const int loss = SufferLoss(m, label);

  if (m * label < 1.0) {
    ++num_update_;

    const double conf = GetConfidence(fv);
    const double beta = 1.0 / (conf + r_);
    const double alpha = (1.0 - label * m) * beta;

    ResizeWeight(fv);

    // Update mean and covariance
    for (fv_t::iterator it = fv.begin(); it != fv.end(); ++it) {
      const unsigned int id = it->first;
      const float val = it->second;
      mean_[id] += alpha * label * cov_[id] * val;
      cov_[id] = 1.f / ((1.f / cov_[id]) + val * val / r_);
    }
  }
}

void BinaryClassifierImpl::ResizeWeight(fv_t &fv) {
  for (std::size_t i = 0; i < fv.size(); ++i) {
    const std::size_t id = fv[i].first;
    if (cov_.size() > id) continue;
    const std::size_t prev_size = cov_.size();

    mean_.resize(id + 1);
    cov_.resize(id + 1);
    for (std::size_t j = prev_size; j < cov_.size(); ++j) {
      mean_[j] = 0.f;
      cov_[j] = 1.f;
    }
  }
}

void BinaryClassifierImpl::Allocate() {
  mean_.reserve(num_feature_ + 1);
  mean_.assign(num_feature_ + 1, 0.f);
  cov_.reserve(num_feature_ + 1);
  cov_.assign(num_feature_ + 1, 1.f);
}

// Get instance
BinaryClassifier *BinaryClassifier::instance() {
  return new BinaryClassifierImpl;
}

// Get instance for SWIG
BinaryClassifier *createBinaryClassifier() {
  return new BinaryClassifierImpl;
}

} // namespace arowpp
