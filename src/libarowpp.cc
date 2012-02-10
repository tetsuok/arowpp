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

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "error_handler.h"
#include "feature.h"
#include "options.h"
#include "scoped_ptr.h"
#include "timer.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

struct arowpp_classifier_t {
  arowpp::BinaryClassifier* ptr;
};

struct arowpp_result_t {
  arowpp::Result* ptr;
};

arowpp_classifier_t* arowpp_classifier_new() {
  arowpp_classifier_t* classifier = new arowpp_classifier_t;
  classifier->ptr = arowpp::BinaryClassifier::instance();
  return classifier;
}

void arowpp_classifier_destroy(arowpp_classifier_t* classifier) {
  delete classifier->ptr;
  delete classifier;
}

int arowpp_train(arowpp_classifier_t* classifier,
                 const char* filename) {
  return classifier->ptr->Train(filename);
}

int arowpp_open(arowpp_classifier_t* classifier,
                const char* filename) {
  return classifier->ptr->Open(filename);
}

int arowpp_load(arowpp_classifier_t* classifier,
                const char* filename) {
  return classifier->ptr->Load(filename);
}

int arowpp_save(const arowpp_classifier_t* classifier,
                const char* filename) {
  return classifier->ptr->Save(filename);
}

int arowpp_classify(arowpp_classifier_t* classifier,
                    const char* line,
                    arowpp_result_t* result) {
  return classifier->ptr->Classify(line, result->ptr);
}

int arowpp_get_num_iter(const arowpp_classifier_t* classifier) {
  return classifier->ptr->get_num_iter();
}

size_t arowpp_get_num_feature(const arowpp_classifier_t* classifier) {
  return classifier->ptr->get_num_feature();
}

size_t arowpp_get_num_example(const arowpp_classifier_t* classifier) {
  return classifier->ptr->get_num_example();
}

size_t arowpp_get_num_update(const arowpp_classifier_t* classifier) {
  return classifier->ptr->get_num_update();
}

double arowpp_get_r(const arowpp_classifier_t* classifier) {
  return classifier->ptr->get_r();
}

int arowpp_is_shuffled(const arowpp_classifier_t* classifier) {
  return classifier->ptr->is_shuffled();
}

void arowpp_set_num_feature(arowpp_classifier_t* classifier,
                            size_t num) {
  classifier->ptr->set_num_feature(num);
}

void arowpp_set_iter(arowpp_classifier_t* classifier,
                     int iter) {
  classifier->ptr->set_num_iter(iter);
}

void arowpp_set_r(arowpp_classifier_t* classifier,
                  double r) {
  classifier->ptr->set_r(r);
}

void arowpp_set_shuffle(arowpp_classifier_t* classifier,
                        int is_shuffled) {
  classifier->ptr->set_shuffle(is_shuffled);
}

const char* arowpp_classifier_error(arowpp_classifier_t* classifier) {
  return classifier->ptr->what();
}

// Result

arowpp_result_t* arowpp_result_new() {
  arowpp_result_t* result = new arowpp_result_t;
  result->ptr = arowpp::Result::instance();
  return result;
}

void arowpp_result_destroy(arowpp_result_t* result) {
  delete result->ptr;
  delete result;
}

void arowpp_result_add(arowpp_result_t* result, short y, short predict) {
  result->ptr->Add(y, predict);
}

void arowpp_result_show(const arowpp_result_t* result) {
  result->ptr->Show();
}

double arowpp_result_calc_accuracy(const arowpp_result_t* result) {
  return result->ptr->CalcAccuracy();
}

unsigned int arowpp_result_get_true_positive(const arowpp_result_t* result) {
  return result->ptr->get_true_positive();
}

unsigned int arowpp_result_get_true_negative(const arowpp_result_t* result) {
  return result->ptr->get_true_negative();
}

unsigned int arowpp_result_get_false_positive(const arowpp_result_t* result) {
  return result->ptr->get_false_positive();
}

unsigned int arowpp_result_get_false_negative(const arowpp_result_t* result) {
  return result->ptr->get_false_negative();
}

unsigned int arowpp_result_get_num_instance(const arowpp_result_t* result) {
  return result->ptr->get_num_instance();
}

unsigned int arowpp_result_get_mistake(const arowpp_result_t* result) {
  return result->ptr->get_mistake();
}


int arow_learn(int argc, char** argv) {
  using namespace arowpp;

  TrainingOptions opts;
  int opt_index;
  CHECK_DIE(ParseCommandLineOptions(argc, argv, &opts, &opt_index))
      << "Cannot parse command line options";

  CHECK_DIE(opt_index + 2 == argc) << "Error invalid argument.";
  CHECK_DIE(opts.num_iter > 0) << "Error: number of iterations must be positive!";
  CHECK_DIE(opts.r > 0.f) << "Error: r must be positive value";

  TinyTimer timer;
  scoped_ptr<arowpp::BinaryClassifier> classifier(arowpp::BinaryClassifier::instance());
  classifier->set_num_iter(opts.num_iter);
  classifier->set_r(opts.r);
  classifier->set_shuffle(opts.enable_shuffle);

  CHECK_DIE(classifier->Train(argv[opt_index])) << "Cannot train "
                                        << classifier->what();
  CHECK_DIE(classifier->Save(argv[opt_index + 1])) << "Cannot save "
                                       << classifier->what();
  std::printf("Done!\nTime: %.4f sec.\n", timer.GetElapsedTime());
  return 0;
}

int arow_test(int argc, char** argv) {
  using namespace arowpp;

  if (argc < 3) {
    std::cerr << "Usage: arow_test test_file model_file" << std::endl;
    return -1;
  }

  const char* test_file = argv[1];
  TinyTimer timer;
  scoped_ptr<arowpp::BinaryClassifier> classifier(arowpp::BinaryClassifier::instance());

  CHECK_DIE(classifier->Load(argv[2])) << "no such file or directory: "
                                                  << classifier->what();

  scoped_ptr<arowpp::Result> result(arowpp::Result::instance());
  unsigned int line_num = 0;
  std::string line;
  std::ifstream ifs(test_file);
  CHECK_DIE(ifs) << "no such file or directory: " << test_file;

  while (std::getline(ifs, line)) {
    if (line[0] == '#' || line.empty()) continue;       // ignore comments

    ++line_num;
    CHECK_DIE(classifier->Classify(line.c_str(), result.get()))
        << "at line: " << line_num << "cannot classifiy: " << classifier->what();
  }

  result->Show();
  std::printf("Done!\nTime: %.4f sec.\n", timer.GetElapsedTime());

  return 0;
}
