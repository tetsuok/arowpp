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

namespace arowpp {

//
// TODO: refactor this class name.
//
class Features {
 public:
  typedef std::vector<std::pair<fv_t, short> >::iterator iterator;
  typedef std::vector<std::pair<fv_t, short> >::const_iterator const_iterator;

  Features() : maxid_(0) {}
  ~Features() {}

  iterator begin() { return features_.begin(); }
  const_iterator begin() const { return features_.begin(); }
  iterator end() { return features_.end(); }
  const_iterator end() const { return features_.end(); }

  std::size_t maxid() const { return maxid_; }

  std::size_t size() const { return features_.size(); }

  bool Open(const char* filename);

  // Shuffle feature vectors
  void Shuffle();

 private:
  // no copying allowed.
  Features(const Features&);
  const Features& operator=(const Features&);

  std::size_t maxid_;                   // Maximum feature id
  std::vector<std::pair<fv_t, short> > features_;
};

} // namespace arowpp
#endif  // AROWPP_FEATURE_H_
