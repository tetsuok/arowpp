#include <gtest/gtest.h>

#include "tokenizer.h"

namespace arowpp {

TEST(TokenizerTest, TokenizeTest) {
  const char line[] = "+1 2:1.8 3:0.4 5:2.0 7:2.1";
  fv_t fv;
  short y = 0;
  size_t dummy = 0;

  EXPECT_TRUE(Tokenizer::Tokenize(line, &fv, &y, &dummy));
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

TEST(TokenizerTest, TokenizeFalseTest) {
  fv_t fv;
  short y = 0;
  size_t dummy = 0;

  {
    const char line[] = "# +1 2:1.2 3:1.0";
    EXPECT_FALSE(Tokenizer::Tokenize(line, &fv, &y, &dummy));
  }

  {
    // Invalid label
    const char line[] = "+2 2:1.2 3:1.0";
    EXPECT_FALSE(Tokenizer::Tokenize(line, &fv, &y, &dummy));
  }
}

} // namespace arowpp
