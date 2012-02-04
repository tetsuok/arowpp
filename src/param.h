// Copyright (c) 2012, Tetsuo Kiso
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

#ifndef AROWPP_PARAM_H_
#define AROWPP_PARAM_H_

namespace arowpp {

// Model parameters
struct Param {
  // Number of features
  unsigned int num_feature;

  // Number of training examples
  std::size_t num_example;

  // Number of updates.
  std::size_t num_update;

  // Number of iteration
  int num_iter;

  // hyperparameter
  double r;

  // Flag for shuffling training examples.
  bool is_shuffled;

  // mean parameters
  std::vector<float> mean;

  // covariance matrix with diagonal elements
  std::vector<float> cov;

  Param()
      : num_feature(0),
        num_example(0),
        num_update(0),
        num_iter(0),
        r(0.1),
        is_shuffled(false) {}

  void Reset() {
    mean.reserve(num_feature + 1);
    mean.assign(num_feature + 1, 0.f);
    cov.reserve(num_feature + 1);
    cov.assign(num_feature + 1, 1.f);
  }
};

} // namespace arowpp

#endif  // AROWPP_PARAM_H_
