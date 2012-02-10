/* Copyright (c) 2010-2012, Tetsuo Kiso
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    * Redistributions of source code must retain the above
 * copyright notice, this list of conditions and the
 * following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 *    * Neither the name of Tetsuo Kiso nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * C and C++ interface.
 */

#ifndef AROWPP_AROWPP_H_
#define AROWPP_AROWPP_H_

/* C interface */
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
# ifdef DLL_EXPORT
#   define AROWPP_DLL_EXTERN __declspec(dllexport)
#   define AROWPP_DLL_CLASS_EXTERN  __declspec(dllexport)
# else
#   define AROWPP_DLL_EXTERN __declspec(dllimport)
# endif  /* DLL_EXPORT */
#endif   /* _WIN32 && !__CYGWIN__ */

#ifndef AROWPP_DLL_EXTERN
#define AROWPP_DLL_EXTERN extern
#endif

#ifndef AROWPP_DLL_CLASS_EXTERN
#define AROWPP_DLL_CLASS_EXTERN
#endif

#ifndef SWIG

/* Exported types */
typedef struct arowpp_classifier_t arowpp_classifier_t;
typedef struct arowpp_result_t arowpp_result_t;

AROWPP_DLL_EXTERN int arow_learn(int argc, char** argv);

AROWPP_DLL_EXTERN int arow_test(int argc, char** argv);

/* Binary classifier */

/* Allocate memory for the object of the BinaryClassifier on the heap.
 * Caller should be delete the returned pointer when it is no longer needed.
 */
AROWPP_DLL_EXTERN arowpp_classifier_t* arowpp_classifier_new();

AROWPP_DLL_EXTERN void arowpp_classifier_destroy(arowpp_classifier_t* classifier);

AROWPP_DLL_EXTERN int arowpp_train(arowpp_classifier_t* classifier,
                                   const char* filename);

AROWPP_DLL_EXTERN int arowpp_open(arowpp_classifier_t* classifier,
                                  const char* filename);

AROWPP_DLL_EXTERN int arowpp_load(arowpp_classifier_t* classifier,
                                  const char* filename);

AROWPP_DLL_EXTERN int arowpp_save(const arowpp_classifier_t* classifier,
                                  const char* filename);

AROWPP_DLL_EXTERN int arowpp_classify(arowpp_classifier_t* classifier,
                                      const char* line,
                                      arowpp_result_t* result);

AROWPP_DLL_EXTERN int arowpp_get_num_iter(const arowpp_classifier_t* classifier);

AROWPP_DLL_EXTERN int arowpp_get_num_feature(const arowpp_classifier_t* classifier);

AROWPP_DLL_EXTERN size_t arowpp_get_num_example(const arowpp_classifier_t* classifier);

AROWPP_DLL_EXTERN size_t arowpp_get_num_update(const arowpp_classifier_t* classifier);

AROWPP_DLL_EXTERN double arowpp_get_r(const arowpp_classifier_t* classifier);

AROWPP_DLL_EXTERN int arowpp_is_shuffled(const arowpp_classifier_t* classifier);

AROWPP_DLL_EXTERN void arowpp_set_num_feature(arowpp_classifier_t* classifier,
                                              unsigned int num);

AROWPP_DLL_EXTERN void arowpp_set_iter(arowpp_classifier_t* classifier,
                                       int iter);

AROWPP_DLL_EXTERN void arowpp_set_r(arowpp_classifier_t* classifier, double r);

AROWPP_DLL_EXTERN void arowpp_set_shuffle(arowpp_classifier_t* classifier,
                                          int is_shuffled);

AROWPP_DLL_EXTERN const char* arowpp_classifier_error(arowpp_classifier_t* classifier);

/* Result */

/* Allocate memory for an object of the Result on the heap.
 * Caller should be delete the returned pointer when it is no longer needed.
 */
AROWPP_DLL_EXTERN arowpp_result_t* arowpp_result_new();

AROWPP_DLL_EXTERN void arowpp_result_destroy(arowpp_result_t* result);

AROWPP_DLL_EXTERN void arowpp_result_add(arowpp_result_t* result,
                                         short y, short label);

AROWPP_DLL_EXTERN void arowpp_result_show(const arowpp_result_t* result);

AROWPP_DLL_EXTERN double arowpp_result_calc_accuracy(const arowpp_result_t* result);

AROWPP_DLL_EXTERN int arowpp_result_get_true_positive(const arowpp_result_t* result);

AROWPP_DLL_EXTERN int arowpp_result_get_true_negative(const arowpp_result_t* result);

AROWPP_DLL_EXTERN int arowpp_result_get_false_positive(const arowpp_result_t* result);

AROWPP_DLL_EXTERN int arowpp_result_get_false_negative(const arowpp_result_t* result);

AROWPP_DLL_EXTERN int arowpp_result_get_num_instance(const arowpp_result_t* result);

AROWPP_DLL_EXTERN int arowpp_result_get_mistake(const arowpp_result_t* result);

#endif  /* SWIG */

#ifdef __cplusplus
} /* extern "C" */
#endif

/* C++ interface */
#ifdef __cplusplus

namespace arowpp {

/* A manager for the result of binary classification problems, used in
 * testing only. It provides methods to get the statistics of
 * the results. In training, it is not needed.
 */
class AROWPP_DLL_CLASS_EXTERN Result {
 public:
  virtual ~Result() {}

  /* Add the result of classification.
   *  y: true label from the data set.
   *  predict: predicted output by the trained classifier.
   */
  virtual bool Add(short y, short predict) = 0;

  /* Print the result of classification problems. */
  virtual void Show() const = 0;

  /* Calculate the accuracy. */
  virtual double CalcAccuracy() const = 0;

  /* Get the number of true positives. */
  virtual unsigned int get_true_positive() const = 0;

  /* Get the number of true negatives. */
  virtual unsigned int get_true_negative() const = 0;

  /* Get the number of false positives. */
  virtual unsigned int get_false_positive() const = 0;

  /* Get the number of false negatives. */
  virtual unsigned int get_false_negative() const = 0;

  /* Get the number of instances classified. */
  virtual unsigned int get_num_instance() const = 0;

  /* Get the number of mistakes. */
  virtual unsigned int get_mistake() const = 0;

  /* An factory method to create a new Result object. */
  static Result* instance();
};

/* This class implements Adaptive Regularization of Weight (AROW)
 * algorithm in the case of binary classification problems.
 *
 * Note: you might want to carefully choose the value of the
 * hyperparameter "r" since the optimal value will depend on the problems.
 *
 * For details, see the paper:
 * Crammer K., Kulesza A. and Dredze M.,
 * "Adaptive Regularization of Weight Vectors". In Proc. of NIPS 2009.
 */
class AROWPP_DLL_CLASS_EXTERN BinaryClassifier {
 public:
  virtual ~BinaryClassifier() {}

  /* Learn the prediction rules from the training examples with specified
   * "filename".
   * Returns true iff the training procedure succeeded.
   */
  virtual bool Train(const char* filename) = 0;

  /* Open the training examples with the specified "filename".
   * Returns true iff the procedure succeeded.
   */
  virtual bool Open(const char* filename) = 0;

  /* Load the trained model with specified "filename".
   * The model file should be binary format.
   * Returns true iff the loading procedure succeeded.
   */
  virtual bool Load(const char* filename) = 0;

  /* Write the trained model to disk.
   * Returns true iff the saving procedure succeeded.
   */
  virtual bool Save(const char* filename) const = 0;

  /* Classify each test example with specified "line".
   * Returns true iff the classification process succeeded.
   */
  virtual bool Classify(const char* line, Result* result) = 0;

  /* Get the number of iterations performed. */
  virtual int get_num_iter() const = 0;

  /* Get the number of features.
   * It means that the maximum feature ID as well.
   */
  virtual unsigned int get_num_feature() const = 0;

  /* Get the number of examples we have ever seen. */
  virtual size_t get_num_example() const = 0;

  /* Get the number of updates. */
  virtual size_t get_num_update() const = 0;

  /* Get the value of the AROW's hyperparameter. */
  virtual double get_r() const = 0;

  /* Retunrs true iff the training examples are randomly shuffled. */
  virtual bool is_shuffled() const = 0;

  /* Set the number of features.
   * It means that we set the maximum feature IDs, and resize
   * the parameter space as well.
   */
  virtual void set_num_feature(unsigned int num_feature) = 0;

  /* Set the number of iterations to be performed. */
  virtual void set_num_iter(int num_iter) = 0;

  /* Set the hyperparameter with specified "r". */
  virtual void set_r(double r) = 0;

  /* Set the flag to randomly shuffle training examples.
   * The default flag is set to false.
   */
  virtual void set_shuffle(bool flag) = 0;

  /* Returns error messages. */
  virtual const char* what() = 0;

  /* An factory method to create a new BinaryClassifier object. */
  static BinaryClassifier* instance();
};

#ifndef SWIG

/* An factory method to create a new Result object.
 * Caller should be delete the returned pointer when it is no longer needed.
 */
AROWPP_DLL_EXTERN Result* createResult();

/* An factory method to create a new BinaryClassifier object.
 * Caller should be delete the returned pointer when it is no longer needed.
 */
AROWPP_DLL_EXTERN BinaryClassifier* createBinaryClassifier();

#endif  /* SWIG */
} /* namespace arowpp */

#endif  /* __cplusplus */
#endif  /* AROWPP_AROWPP_H_ */
