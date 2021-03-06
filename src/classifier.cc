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
#include <memory>
#include "tokenizer.h"
#include "param.h"
#include "model_reader_interface.h"
#include "model_writer_interface.h"
#include "logging.h"

namespace arowpp {

class Features;
class Result;

class BinaryClassifierImpl : public BinaryClassifier {
 public:
  BinaryClassifierImpl()
      : param_(new Param),
        features_(new Features) {}

  virtual ~BinaryClassifierImpl() {}

  virtual bool Train(const char* filename);
  virtual bool Open(const char* filename);
  virtual bool Save(const char* filename) const;
  virtual bool Load(const char* filename);
  virtual bool Classify(const char* line, Result* result);

  virtual int get_num_iter() const { return param_->num_iter; }
  virtual size_t get_num_feature() const { return param_->num_feature; }
  virtual size_t get_num_example() const { return param_->num_example; }
  virtual size_t get_num_update() const { return param_->num_update; }
  virtual double get_r() const { return param_->r; }
  virtual bool is_shuffled() const { return param_->is_shuffled; }

  virtual void set_num_feature(size_t num_feature) {
    param_->num_feature = num_feature;
  }

  virtual void set_num_iter(int num_iter) {
    param_->num_iter = num_iter;
  }

  virtual void set_r(double r) { param_->r = r; }
  virtual void set_shuffle(bool flag) { param_->is_shuffled = flag; }

  virtual const char* what() { return what_.str(); }

 private:
  // Updates mean and confindence parameters.
  void Update(const fv_t &fv, short label);

  // Makes a prediction from sign of margin.
  // Returns the predicted label.
  short Predict(const fv_t &fv) {
    ResizeWeight(fv);
    const double m = GetMargin(fv);
    return (m > 0.0) ? 1 : -1;
  }

  double GetMargin(const fv_t& fv) const {
    double res = 0.0;
    for (const auto& elem : fv) {
      res += param_->mean[elem.first] * elem.second;
    }
    return res;
  }

  double GetConfidence(const fv_t& fv) const {
    double res = 0.0;
    for (const auto& elem : fv) {
      res += param_->cov[elem.first] * elem.second * elem.second;
    }
    return res;
  }

  // Resizes mean and covariance parameters.
  void ResizeWeight(const fv_t &fv);

  // We disable the default assignment operator and copy constructor.
  BinaryClassifierImpl(const BinaryClassifierImpl&);
  const BinaryClassifierImpl& operator=(const BinaryClassifierImpl&);

  std::unique_ptr<Param> param_;
  std::unique_ptr<Features> features_;       // Feature vector
  whatlog what_;                        // store error log
};

bool BinaryClassifierImpl::Train(const char *filename) {
  CHECK_FALSE(Open(filename)) << "Cannot read " << filename;

  if (is_shuffled()) {
    features_->Shuffle();
  }

  for (int i = 0; i < get_num_iter(); ++i) {
    for (Features::const_iterator it = features_->begin();
         it != features_->end(); ++it) {
      Update(it->first, it->second);
    }
  }
  return true;
}

bool BinaryClassifierImpl::Open(const char *filename) {
  CHECK_FALSE(features_->Open(filename)) << "no such file or directory: "
                                         << filename;
  set_num_feature(features_->maxid());
  param_->Reset();
  return true;
}

bool BinaryClassifierImpl::Save(const char *filename) const {
  if (param_->num_example == 0) {
    LOG(ERROR) << "Could not train examples";
    return false;
  }

  std::unique_ptr<ModelWriterInterface> writer(
      ModelWriterFactory::GetModelWriter());

  if (!writer->Open(filename, param_.get())) {
    LOG(ERROR) << "Failed to save trained model to disk.";
    return false;
  }

  std::cout << "Number of features: " << param_->num_feature << "\n"
            << "Number of examples: "
            << static_cast<double>(param_->num_example) / param_->num_iter << "\n"
            << "Number of updates:  " << param_->num_update << std::endl;

  return true;
}

bool BinaryClassifierImpl::Load(const char *filename) {
  std::unique_ptr<ModelReaderInterface> reader(
      ModelReaderFactory::GetModelReader());
  Param* param = new Param;
  CHECK_FALSE(reader->Open(filename, param))
      << "Failed to load model " << filename;
  param_.reset(param);
  return true;
}

inline bool BinaryClassifierImpl::Classify(const char* line,
                                           Result* result) {
  short label = 0;                      // true label
  size_t dummy = 0;                     // dummy id

  const fv_t fv = Tokenizer::Tokenize(line, &label, &dummy);
  if (!fv.size()) {
    LOG(ERROR) << "Cannot classify";
    return false;
  }

  if (!result->Add(label, Predict(fv))) {
    LOG(ERROR) << "Failed to count the classified example.";
    return false;
  }

  return true;
}

void BinaryClassifierImpl::Update(const fv_t &fv, short label) {
  ++param_->num_example;
  const double m = GetMargin(fv);

  if (m * label < 1.0) {
    ++param_->num_update;

    const double conf = GetConfidence(fv);
    const double beta = 1.0 / (conf + param_->r);
    const double alpha = (1.0 - label * m) * beta;

    ResizeWeight(fv);

    // Update mean and covariance
    for (const auto& elem : fv) {
      const unsigned int id = elem.first;
      const float val = elem.second;
      param_->mean[id] += alpha * label * param_->cov[id] * val;
      param_->cov[id] = 1.f / ((1.f / param_->cov[id]) + val * val / static_cast<float>(param_->r));
    }
  }
}

void BinaryClassifierImpl::ResizeWeight(const fv_t &fv) {
  for (auto f : fv) {
    const std::size_t id = f.first;
    if (param_->cov.size() > id) continue;
    const std::size_t prev_size = param_->cov.size();

    param_->mean.resize(id + 1);
    param_->cov.resize(id + 1);
    for (std::size_t j = prev_size; j < param_->cov.size(); ++j) {
      param_->mean[j] = 0.f;
      param_->cov[j] = 1.f;
    }
  }
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
