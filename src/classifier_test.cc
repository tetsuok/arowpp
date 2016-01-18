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

#include "arowpp.h"
#include <memory>
#include <gtest/gtest.h>

namespace arowpp {

class BinaryClassifierTest : public testing::Test {
 protected:
  virtual void SetUp() {
    classifier_.reset(BinaryClassifier::instance());
  }

  std::unique_ptr<BinaryClassifier> classifier_;
};

TEST_F(BinaryClassifierTest, SetParameterTest) {
  int num_iter = 10;
  double r = 2.0;
  bool shuffle = true;

  classifier_->set_num_iter(num_iter);
  classifier_->set_r(r);
  classifier_->set_shuffle(shuffle);

  EXPECT_EQ(num_iter, classifier_->get_num_iter());
  EXPECT_FLOAT_EQ(r, classifier_->get_r());
  EXPECT_TRUE(classifier_->is_shuffled());
}

// FIXME: temporarily turn off until we figure out how to specify the dependency
// on the resource file in BUILD.
#if 0
TEST_F(BinaryClassifierTest, TrainTest) {
  const char kTrainFile[] = "../sample/train.dat";
  EXPECT_TRUE(classifier_->Train(kTrainFile));
}
#endif

} // namespace arowpp
