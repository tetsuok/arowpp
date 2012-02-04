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
#include "scoped_ptr.h"
#include <gtest/gtest.h>

namespace arowpp {

class ResultTest : public testing::Test {
 protected:
  virtual void SetUp() {
    result_.reset(Result::instance());
  }

  scoped_ptr<Result> result_;
};

TEST_F(ResultTest, AddTest) {
  EXPECT_TRUE(result_->Add(1, 1));      // tp
  EXPECT_TRUE(result_->Add(-1, -1));    // tn
  EXPECT_TRUE(result_->Add(-1, 1));     // fp
  EXPECT_TRUE(result_->Add(1, -1));     // fn

  // Invalid cases
  EXPECT_FALSE(result_->Add(0, 1));
  EXPECT_FALSE(result_->Add(1, -2));

  EXPECT_EQ(4, result_->get_num_instance());
  EXPECT_EQ(2, result_->get_mistake());
  EXPECT_EQ(1, result_->get_true_positive());
  EXPECT_EQ(1, result_->get_true_negative());
  EXPECT_EQ(1, result_->get_false_positive());
  EXPECT_EQ(1, result_->get_false_negative());
}

} // namespace arowpp
