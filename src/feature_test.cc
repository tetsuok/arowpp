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

#include "feature.h"
#include "scoped_ptr.h"
#include <gtest/gtest.h>

namespace {

const char kTrainFile[] = "../sample/train.dat";

} // namespace

namespace arowpp {

class FeaturesTest : public testing::Test {
 protected:
  virtual void SetUp() {
    features_.reset(new Features);
  }

  scoped_ptr<Features> features_;
};

TEST_F(FeaturesTest, OpenTest) {
  ASSERT_TRUE(features_->Open(kTrainFile));
  EXPECT_TRUE(features_->maxid() > 0);
  EXPECT_TRUE(features_->size() > 0);

  // Verification labels
  for (std::size_t i = 0; i < features_->size(); ++i) {
    std::pair<fv_t, short>& x = features_->get_instance(i);
    EXPECT_TRUE(x.second == 1 || x.second == -1);
  }
}


TEST_F(FeaturesTest, ParseLineTest) {
  const char line[] = "+1 2:1.8 3:0.4 5:2.0 7:2.1";
  fv_t fv;
  short y = 0;

  EXPECT_TRUE(features_->ParseLine(line, fv, y));
  EXPECT_EQ(+1, y);
  EXPECT_EQ(4, fv.size());

  EXPECT_EQ(2, fv[0].first);
  EXPECT_FLOAT_EQ(1.8, fv[0].second);
  EXPECT_EQ(3, fv[1].first);
  EXPECT_FLOAT_EQ(0.4, fv[1].second);
  EXPECT_EQ(5, fv[2].first);
  EXPECT_FLOAT_EQ(2.0, fv[2].second);
  EXPECT_EQ(7, fv[3].first);
  EXPECT_FLOAT_EQ(2.1, fv[3].second);
}

TEST_F(FeaturesTest, ParseLineFalseTest) {
  fv_t fv;
  short y = 0;

  {
    const char line[] = "# +1 2:1.2 3:1.0";
    EXPECT_FALSE(features_->ParseLine(line, fv, y));
  }

  {
    // Invalid label
    const char line[] = "+2 2:1.2 3:1.0";
    EXPECT_FALSE(features_->ParseLine(line, fv, y));
  }
}

TEST(ParseTest, BasicTest) {
  const char line[] = "+1 2:1.8 3:0.4 5:2.0 7:2.1";
  fv_t fv;
  short y = 0;

  EXPECT_TRUE(Features::Parse(line, fv, y));
  EXPECT_EQ(+1, y);
  EXPECT_EQ(4, fv.size());

  EXPECT_EQ(2, fv[0].first);
  EXPECT_FLOAT_EQ(1.8, fv[0].second);
  EXPECT_EQ(3, fv[1].first);
  EXPECT_FLOAT_EQ(0.4, fv[1].second);
  EXPECT_EQ(5, fv[2].first);
  EXPECT_FLOAT_EQ(2.0, fv[2].second);
  EXPECT_EQ(7, fv[3].first);
  EXPECT_FLOAT_EQ(2.1, fv[3].second);
}

TEST(FeatureFalseTest, InvalidLabelTest) {
  fv_t fv;
  short y = 0;

  {
    const char line[] = "# +1 2:1.2 3:1.0";
    EXPECT_FALSE(Features::Parse(line, fv, y));
  }

  {
    const char line[] = "+2 2:1.2 3:1.0";
    EXPECT_FALSE(Features::Parse(line, fv, y));
  }
}
} // namespace arowpp
