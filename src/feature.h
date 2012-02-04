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

#ifndef AROWPP_FEATURE_H_
#define AROWPP_FEATURE_H_

#include "common.h"
#include <cstring>
#include <ctype.h>

namespace arowpp {

//
// TODO: refactor this class name.
//
class Features {
 public:
  Features() : maxid_(0) {}
  ~Features() {}

  std::pair<fv_t, short>& get_instance(int i) { return features_[i]; }

  std::size_t maxid() const { return maxid_; }

  std::size_t size() const { return features_.size(); }

  bool Open(const char* filename);

  // Construct the feature vectors from parsed lines
  bool ParseLine(const char* line, fv_t& fv, short& y) {
    CHECK_FALSE(line[0] != '#');
    y = static_cast<short>(atoi(line));

    CHECK_FALSE(y == 1 || y == -1) << "parse error: y is not +1 nor -1";

    while (!isspace(*line) && *line) line++;
    while (isspace(*line) && *line) line++;
    while (1) {
      const char *begin = line;
      unsigned int index = std::atol(begin);
      while (*line != ':' && *line) line++;
      float val = std::atof(++line);

      while (*line != ' ' && *line) line++;
      fv.push_back(std::make_pair(index, val));
      if (index > maxid_) maxid_ = index;
      if (*line++ == 0) break;
    }
    return true;
  }

  // TODO: This is inefficient.
  // for testing
  static bool Parse(const char* line, fv_t& fv, short& y) {
    Features f;
    return f.ParseLine(line, fv, y);
  }

  // Shuffle feature vectors
  void Shuffle();

  const char* what() { return what_.str(); }

 private:
  Features(const Features&);
  const Features& operator=(const Features&);

  std::size_t maxid_;                   // Maximum feature id
  std::vector<std::pair<fv_t, short> > features_;
  whatlog what_;
};

} // namespace arowpp
#endif  // AROWPP_FEATURE_H_
