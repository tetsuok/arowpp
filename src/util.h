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

#ifndef AROWPP_UTIL_H_
#define AROWPP_UTIL_H_

#include "common.h"

namespace arowpp {

static const char* PROGRAM_NAME = "arow_learn";

void UsageTrainer() {
  std::cerr << "Usage: " << PROGRAM_NAME
            << " train_file model_file -i=num_iter -r=double -s\n"
            << "option\n"
            << "-i: number of iteration (Default 1)\n"
            << "-r: hyper parameter (Default 0.1)\n"
            << "-s: flag to shuffle training data (Default do not shuffle)"
            << std::endl;
}

void UsageClassifier() {
  std::cerr << "Usage: arow_test test_file model_file" << std::endl;
}

// Parer command line options.
// This class is experimental.
class OptionParser {
 public:
  explicit OptionParser(int index) : begin_index_(index) {}

  ~OptionParser() {}

  void Parse(int argc, char** argv, int& num_iter, double& r, bool& shuffle) {
    for (int i = begin_index_; i < argc; ++i) {
      CHECK_DIE(parse_str(argv[i], num_iter, r, shuffle) == 0) << "Parse error ";
    }
  }

 private:
  int parse_str(const char* s, int& num_iter, double& r, bool& shuffle) {
    std::string str(s);

    if (str.size() < 2)
      return 0;
    else if (str.substr(0, 3) == "-i=") {
      num_iter = atoi(str.substr(3).c_str());
      return 0;
    }
    else if (str.substr(0, 3) == "-r=") {
      r = atof(str.substr(3).c_str());
      return 0;
    }
    else if (str.substr(0, 2) == "-h") {
      UsageTrainer();
      std::exit(-1);
    }
    else if (str.substr(0, 2) == "-s") {
      shuffle = true;
      return 0;
    }
    else {
      std::cerr << "Unknown option: " << str.substr(0, 3) << std::endl;
      return -1;
    }
  }

  int begin_index_;
  whatlog what_;
};
} // namespace arowpp

#endif  // AROWPP_UTIL_H_
