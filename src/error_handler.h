// Copyright (c) 2005-2007, Taku Kudo
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
//  * Neither the name of Taku Kudo nor the names of its contributors
//    may be used to endorse or promote products derived from this
//    software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include <csetjmp>
#include <string>
#include <iostream>

namespace arowpp {

class die {
 public:
  die() {}
  ~die() { std::cerr << std::endl; exit(-1); }
  int operator&(std::ostream&) { return 0; }
};

class warn {
 public:
  warn() {}
  ~warn() { std::cerr << std::endl; }
  int operator&(std::ostream&) { return 0; }
};

struct whatlog {
  std::ostringstream stream_;
  std::string str_;
  std::jmp_buf cond_;

  const char *str() {
    str_ = stream_.str();
    return str_.c_str();
  }
};

class wlog {
 public:
  whatlog *l_;
  explicit wlog(whatlog *l): l_(l) { l_->stream_.clear(); }
  ~wlog() { std::longjmp(l_->cond_, 1); }
  int operator&(std::ostream &) { return 0; }
};
} // namespace arowpp

#define WHAT what_.stream_

#define CHECK_RETURN(condition, value)                                  \
  if (condition) {} else                                                \
    if (setjmp(what_.cond_) == 1) {                                     \
      return value;                                                     \
    } else                                                              \
      wlog(&what_) & what_.stream_ <<                                   \
          __FILE__ << "(" << __LINE__ << ") [" << #condition << "] "

#define CHECK_0(condition)      CHECK_RETURN(condition, 0)
#define CHECK_FALSE(condition)  CHECK_RETURN(condition, false)

#define CHECK_CLOSE_FALSE(condition)                                    \
  if (condition) {} else                                                \
    if (setjmp(what_.cond_) == 1) {                                     \
      close();                                                          \
      return false;                                                     \
    } else                                                              \
      wlog(&what_) & what_.stream_ <<                                   \
          __FILE__ << "(" << __LINE__ << ") [" << #condition << "] "

#define CHECK_DIE(condition)                                            \
  (condition) ? 0 : die() & std::cerr << __FILE__ <<                    \
    "(" << __LINE__ << ") [" << #condition << "] "

#define CHECK_WARN(condition)                                           \
  (condition) ? 0 : warn() & std::cerr << __FILE__ <<                   \
    "(" << __LINE__ << ") [" << #condition << "] "

#endif  // _ERROR_HANDLER_H_
