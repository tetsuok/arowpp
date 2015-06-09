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

#include "options.h"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

namespace arowpp {
namespace {

template <class Target, class Source>
Target lexical_cast(Source arg) {
  std::stringstream ss;
  Target ret;
  if (!(ss << arg) || !(ss >> ret) || !(ss >> std::ws).eof()) {
    std::unique_ptr<Target> r(new Target());
    return *r;
  }
  return ret;
}

const int kDefaultNumIter = 1;
const double kDefaultR = 0.1;

} // namespace

TrainingOptions::TrainingOptions()
  : num_iter(kDefaultNumIter),
    r(kDefaultR),
    enable_shuffle(false) {}

void PrintUsage(const char* cmd) {
  std::cerr << "Usage: " << cmd
            << "-i num_iter -r hyp -s train_file model_file\n"
            << "Available options:\n"
            << "-i: number of iteration (default " << kDefaultNumIter << ")\n"
            << "-r: hyperparameter (default " << kDefaultR << ")\n"
            << "-s: flag to shuffle training data (default false)\n";
}

bool ParseCommandLineOptions(int argc, char** argv,
                             TrainingOptions* opts, int* opt_index) {
  if (argc < 3) {
    PrintUsage(argv[0]);
    return false;
  }

  int opt;
  while ((opt = getopt(argc, argv, "si:r:")) != -1) {
    switch (opt) {
      case 's':
        opts->enable_shuffle = true;
        break;
      case 'i':
        opts->num_iter = lexical_cast<int, std::string>(std::string(optarg));
        break;
      case 'r':
        opts->r = lexical_cast<double, std::string>(std::string(optarg));
        break;
      default:
        PrintUsage(argv[0]);
        return false;
    }
  }
  *opt_index = optind;

  return true;
}

} // namespace arowpp
