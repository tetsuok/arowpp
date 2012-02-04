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

#include <gtest/gtest.h>
#include "model_reader_interface.h"
#include "model_writer_interface.h"
#include "scoped_ptr.h"
#include "param.h"

namespace arowpp {
namespace {

const char kFilename[] = "/tmp/test.model";

} // namespace

TEST(ModelReaderTest, BasicTest) {
  // Setup
  scoped_ptr<ModelWriterInterface> writer(
      ModelWriterFactory::GetModelWriter());
  Param param;
  param.num_feature = 2012;
  param.num_example = 1985;
  param.num_update = 29;
  param.num_iter = 1;
  param.r = 0.1;
  EXPECT_TRUE(writer->Open(kFilename, &param));

  scoped_ptr<ModelReaderInterface> reader(
      ModelReaderFactory::GetModelReader());
  Param param2;
  EXPECT_TRUE(reader->Open(kFilename, &param2));

  EXPECT_EQ(param.num_feature, param2.num_feature);
  EXPECT_EQ(param.num_example, param2.num_example);
  EXPECT_EQ(param.num_update, param2.num_update);
  EXPECT_EQ(param.num_iter, param2.num_iter);
  EXPECT_DOUBLE_EQ(param.r, param2.r);
  EXPECT_TRUE(param.is_shuffled == param2.is_shuffled);
}

} // namespace arowpp
