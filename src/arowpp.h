//  Copyright (c) 2010-2012, Tetsuo Kiso
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
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

#ifndef AROWPP_AROWPP_H_
#define AROWPP_AROWPP_H_

// C interface
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AROWPP_DLL_EXTERN
#define AROWPP_DLL_EXTERN extern
#endif

#ifndef SWIG
  typedef struct arowpp_classifier_t arowpp_classifier_t;
  typedef struct arowpp_result_t arowpp_result_t;

  // C interface
  AROWPP_DLL_EXTERN int arow_learn(int argc, char** argv);
  AROWPP_DLL_EXTERN int arow_test(int argc, char** argv);

  // classifier
  AROWPP_DLL_EXTERN arowpp_classifier_t* arowpp_classifier_new();

  AROWPP_DLL_EXTERN void arowpp_classifier_destroy(arowpp_classifier_t* classifier);

  AROWPP_DLL_EXTERN int arowpp_train(arowpp_classifier_t* classifier,
                                     const char* filename);

  AROWPP_DLL_EXTERN int arowpp_open(arowpp_classifier_t* classifier,
                                    const char* filename);

  AROWPP_DLL_EXTERN int arowpp_load(arowpp_classifier_t* classifier,
                                    const char* filename);

  AROWPP_DLL_EXTERN int arowpp_save(arowpp_classifier_t* classifier,
                                    const char* filename);

  AROWPP_DLL_EXTERN int arowpp_classify(arowpp_classifier_t* classifier,
                                        const char* line,
                                        arowpp_result_t* result);

  AROWPP_DLL_EXTERN int arowpp_get_num_iter(arowpp_classifier_t* classifier);

  AROWPP_DLL_EXTERN int arowpp_get_num_feature(arowpp_classifier_t* classifier);

  AROWPP_DLL_EXTERN size_t arowpp_get_num_example(arowpp_classifier_t* classifier);

  AROWPP_DLL_EXTERN size_t arowpp_get_num_update(arowpp_classifier_t* classifier);

  AROWPP_DLL_EXTERN double arowpp_get_r(arowpp_classifier_t* classifier);

  AROWPP_DLL_EXTERN int arowpp_is_shuffled(arowpp_classifier_t* classifier);

  AROWPP_DLL_EXTERN void arowpp_set_num_feature(arowpp_classifier_t* classifier,
                                                unsigned int num);

  AROWPP_DLL_EXTERN void arowpp_set_iter(arowpp_classifier_t* classifier,
                                         int iter);

  AROWPP_DLL_EXTERN void arowpp_set_r(arowpp_classifier_t* classifier, double r);

  AROWPP_DLL_EXTERN void arowpp_set_shuffle(arowpp_classifier_t* classifier,
                                            int is_shuffled);

  AROWPP_DLL_EXTERN const char* arowpp_classifier_error(arowpp_classifier_t* classifier);

  // Result
  AROWPP_DLL_EXTERN arowpp_result_t* arowpp_result_new();

  AROWPP_DLL_EXTERN void arowpp_result_destroy(arowpp_result_t* result);

  AROWPP_DLL_EXTERN void arowpp_result_add(arowpp_result_t* result,
                                           short y, short label);

  AROWPP_DLL_EXTERN void arowpp_result_show(arowpp_result_t* result);

  AROWPP_DLL_EXTERN int arowpp_result_get_true_positive(arowpp_result_t* result);

  AROWPP_DLL_EXTERN int arowpp_result_get_true_negative(arowpp_result_t* result);

  AROWPP_DLL_EXTERN int arowpp_result_get_false_positive(arowpp_result_t* result);

  AROWPP_DLL_EXTERN int arowpp_result_get_false_negative(arowpp_result_t* result);

  AROWPP_DLL_EXTERN int arowpp_result_get_num_instance(arowpp_result_t* result);

  AROWPP_DLL_EXTERN int arowpp_result_get_mistake(arowpp_result_t* result);


#endif  // SWIG

#ifdef __cplusplus
}
#endif

// C++ interface
#ifdef __cplusplus

namespace arowpp {

// Result of classification
class Result {
 public:
  virtual ~Result() {}

  // Add result of classification
  // y: true label
  // predict: predicted label
  virtual bool Add(short y, short predict) = 0;

  // Print result of classification
  virtual void Show() = 0;

  // Get true positive
  virtual unsigned int get_true_positive() const = 0;

  // Get true negative
  virtual unsigned int get_true_negative() const = 0;

  // Get false positive
  virtual unsigned int get_false_positive() const = 0;

  // Get false negative
  virtual unsigned int get_false_negative() const = 0;

// Get the number of instances classified
  virtual unsigned int get_num_instance() const = 0;

  // Get number of mistakes
  virtual unsigned int get_mistake() const = 0;

  static Result* instance();
};


// This class is an implementation of Adaptive Regularization
// of Weight (AROW) algorithm that is a binary classifier.
// For details, see:
// Crammer K., Kulesza A. and Dredze M.,
// "Adaptive Regularization of Weight Vectors". NIPS 2009.
class BinaryClassifier {
 public:
  virtual ~BinaryClassifier() {}

  // Learn the prediction rule from training examples.
  virtual bool Train(const char* filename) = 0;

  // Open training data.
  virtual bool Open(const char* filename) = 0;

  // Load model file.
  virtual bool Load(const char* filename) = 0;

  // Save model file.
  virtual bool Save(const char* filename) = 0;

  // Predict label for test examples.
  virtual bool Classify(const char* line, Result* result) = 0;

  // Get number of iteration.
  virtual int get_num_iter() const = 0;

  // Get number of features.
  virtual unsigned int get_num_feature() const = 0;

  // Get number of examples.
  virtual size_t get_num_example() const = 0;

  // Get number of update.
  virtual size_t get_num_update() const = 0;

  // Get hyper parameter.
  virtual double get_r() const = 0;

  // Has the training data been shuffled?
  virtual bool is_shuffled() const = 0;

  // Set the number of features.
  virtual void set_num_feature(unsigned int num_feature) = 0;

  // Set the number of iterations.
  virtual void set_num_iter(int num_iter) = 0;

  // Set the hyperparameter.
  virtual void set_r(double r) = 0;

  // Enable shuffling training examples.
  virtual void set_shuffle(bool flag) = 0;

  virtual const char* what() = 0;

  static BinaryClassifier* instance();
};

#ifndef SWIG
AROWPP_DLL_EXTERN Result* createResult();
AROWPP_DLL_EXTERN BinaryClassifier* createBinaryClassifier();
#endif  // SWIG
} // namespace arowpp

#endif  // __cplusplus
#endif  // AROWPP_AROWPP_H_
