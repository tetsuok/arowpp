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

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "arowpp.h"
#include "scoped_ptr.h"
#include "error_handler.h"
#include "timer.h"
#include "util.h"
#include "feature.h"

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

int arowpp_save(arowpp_classifier_t* classifier,
                const char* filename) {
  return classifier->ptr->Save(filename);
}

int arowpp_classify(arowpp_classifier_t* classifier,
                    const char* line,
                    arowpp_result_t* result) {
  return classifier->ptr->Classify(line, result->ptr);
}

int arowpp_get_num_iter(arowpp_classifier_t* classifier) {
  return classifier->ptr->get_num_iter();
}

int arowpp_get_num_feature(arowpp_classifier_t* classifier) {
  return classifier->ptr->get_num_feature();
}

size_t arowpp_get_num_example(arowpp_classifier_t* classifier) {
  return classifier->ptr->get_num_example();
}

size_t arowpp_get_num_update(arowpp_classifier_t* classifier) {
  return classifier->ptr->get_num_update();
}

double arowpp_get_r(arowpp_classifier_t* classifier) {
  return classifier->ptr->get_r();
}

int arowpp_is_shuffled(arowpp_classifier_t* classifier) {
  return classifier->ptr->is_shuffled();
}

void arowpp_set_num_feature(arowpp_classifier_t* classifier,
                            int num) {
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

void arowpp_result_show(arowpp_result_t* result) {
  result->ptr->Show();
}

int arowpp_result_get_true_positive(arowpp_result_t* result) {
  return result->ptr->get_true_positive();
}

int arowpp_result_get_true_negative(arowpp_result_t* result) {
  return result->ptr->get_true_negative();
}

int arowpp_result_get_false_positive(arowpp_result_t* result) {
  return result->ptr->get_false_positive();
}

int arowpp_result_get_false_negative(arowpp_result_t* result) {
  return result->ptr->get_false_negative();
}

int arowpp_result_get_num_instance(arowpp_result_t* result) {
  return result->ptr->get_num_instance();
}

int arowpp_result_get_mistake(arowpp_result_t* result) {
  return result->ptr->get_mistake();
}


int arow_learn(int argc, char** argv) {
  using namespace arowpp;

  // TODO: refactoring
  if (argc < 3) {
    UsageTrainer();
    return -1;
  }

  int num_iter = 1;            // Number of iterations
  double r = 0.1;              // hyperparameter
  bool is_shuffled = false;        // Shuffle data?

  TinyTimer timer;
  OptionParser parser(3);
  parser.Parse(argc, argv, num_iter, r, is_shuffled);


  CHECK_DIE(num_iter > 0) << "Error: number of iterations must be positive!";
  CHECK_DIE(r > 0.f) << "Error: r must be positive value";

  scoped_ptr<arowpp::BinaryClassifier> classifier(arowpp::BinaryClassifier::instance());
  classifier->set_num_iter(num_iter);
  classifier->set_r(r);
  classifier->set_shuffle(is_shuffled);

  CHECK_DIE(classifier->Train(argv[1])) << "Cannot train "
                                        << classifier->what();
  CHECK_DIE(classifier->Save(argv[2])) << "Cannot save "
                                       << classifier->what();
  std::printf("Done!\nTime: %.4f sec.\n", timer.GetElapsedTime());
  return 0;
}

int arow_test(int argc, char** argv) {
  using namespace arowpp;

  if (argc < 3) {
    UsageClassifier();
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
