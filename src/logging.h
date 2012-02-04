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

#ifndef AROWPP_LOGGING_H_
#define AROWPP_LOGGING_H_

#include <cstdlib>
#include <fstream>
#include <iostream>

namespace logging {

enum LogLevel {
  LOG_INFO = 0,
  LOG_WARNING = 1,
  LOG_ERROR = 2,
  LOG_FATAL = 3
};

class Logger {
 public:
  static std::ostream &GetLogStream() {
    return std::cerr;
  }

 private:
  Logger() {}
  virtual ~Logger() {}
};

class LogFinalizer {
 public:
  explicit LogFinalizer(LogLevel level) : level_(level) {}
  ~LogFinalizer() {
    Logger::GetLogStream() << std::endl;
    if (level_ == LOG_FATAL) {
      std::exit(-1);
    }
  }

  // To ignore values used in when defining logging macros.
  void operator&(std::ostream&) {}

 private:
  LogLevel level_;
};
} // namespace logging

#define CHECK_AND_DIE(condition) \
  (condition) ? (void) 0 : logging::LogFinalizer(logging::LOG_FATAL) & \
  logging::Logger::GetLogStream() << "Die: " << \
  __FILE__ << "(" << __LINE__ << ") [" << #condition << "] "

#define LOG(loglevel) \
  logging::LogFinalizer(logging::LOG_##loglevel) & \
  logging::Logger::GetLogStream() \
  << "LOG(" <<  #loglevel << "): " << __FILE__ << "(" << __LINE__ << ") "

#endif  // AROWPP_LOGGING_H_
