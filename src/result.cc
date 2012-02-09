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

#include <cstdio>
#include <iostream>
#include <vector>
#include "arowpp.h"

namespace arowpp {

class ResultImpl : public Result {
 public:
  ResultImpl() : num_instance_(0), mistake_(0) {
    results_.clear();
    results_.resize(4);
    results_.assign(4, 0);
  }

  virtual ~ResultImpl() { results_.clear(); }

  virtual bool Add(short y, short predict) {
    if      (y ==  1 && predict ==  1) { results_[0]++; }
    else if (y == -1 && predict == -1) { results_[1]++; }
    else if (y == -1 && predict ==  1) { ++mistake_; results_[2]++; }
    else if (y ==  1 && predict == -1) { ++mistake_; results_[3]++; }
    else                               { return false;  }

    ++num_instance_;

    return true;
  }

  virtual void Show() const;

  virtual unsigned int get_true_positive() const { return results_[0]; }
  virtual unsigned int get_true_negative() const { return results_[1]; }
  virtual unsigned int get_false_positive() const { return results_[2]; }
  virtual unsigned int get_false_negative() const { return results_[3]; }

  virtual unsigned int get_num_instance() const { return num_instance_; }

  virtual unsigned int get_mistake() const { return mistake_; }

 private:
  unsigned int num_instance_;                    // Number of classified instance
  unsigned int mistake_;                         // Number of mistakes
  std::vector<unsigned int> results_;            // results of classification
};

void ResultImpl::Show() const {
  std::printf("Accuracy %.3f%% (%d/%d)\n(Answer, Predict): "
              "(t,p):%d (t,n):%d (f,p):%d (f,n):%d\n",
              static_cast<double>((num_instance_ - mistake_) * 100.0 / num_instance_),
              num_instance_ - mistake_, num_instance_,
              results_[0], results_[1], results_[2], results_[3]);
}

// Get instance
Result* Result::instance() {
  return new ResultImpl;
}

// Get instance for SWIG
Result* createResult() {
  return new ResultImpl;
}

} // namespace arowpp
