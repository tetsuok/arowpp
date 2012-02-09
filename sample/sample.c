/* Copyright (c) 2010-2012, Tetsuo Kiso
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the
 *    following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 *  * Neither the name of Tetsuo Kiso nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 */

/* A sample C program */

#include "arowpp.h"

#include <stdlib.h>
#include <string.h>

/* You have to set this size according to the data set. */
/* #define BUF_SIZE 65536 */
/* #define BUF_SIZE 131072 */

#define BUF_SIZE 262144

/* gcc sample.c -larowpp -lstdc++ */

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s train_file test_file\n", argv[0]);
        exit(1);
    }
    arowpp_classifier_t *classifier = arowpp_classifier_new();
    arowpp_result_t *result = arowpp_result_new();

    const char *train_file = argv[1];
    const char *test_file = argv[2];
    const char *model_file = "model";

    FILE *fp;
    char buff[BUF_SIZE];
    unsigned int line_num = 0;

    arowpp_set_iter(classifier, 1);
    arowpp_set_r(classifier, 2.f);

    /* Training */
    if (!arowpp_train(classifier, train_file)) {
        fprintf(stderr, "Cannot train %s\n", arowpp_classifier_error(classifier));
        exit(1);
    }

    /* Save model file */
    if (!arowpp_save(classifier, model_file)) {
        fprintf(stderr, "Cannot train %s\n", arowpp_classifier_error(classifier));
        return -1;
    }

    printf("Number of iteration: %d\n", arowpp_get_num_iter(classifier));

    /* Load model file */
    if (!arowpp_load(classifier, model_file)) {
        fprintf(stderr, "Cannot train %s\n", arowpp_classifier_error(classifier));
        return -1;
    }

    /* Test */
    fp = fopen(test_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open %s\n", test_file);
        exit(1);
    }

    while (fgets(buff, sizeof(buff), fp)) {
        if (buff[0] == '#' || buff[0] == '\n') continue;

        line_num++;
        if (!arowpp_classify(classifier, buff, result)) {
            fprintf(stderr, "Cannot classify %s\n",
                    arowpp_classifier_error(classifier));
            exit(1);
        }
    }
    fclose(fp);

    /* Show result */
    printf("Number of classified: %d\n", arowpp_result_get_num_instance(result));
    arowpp_result_show(result);

    arowpp_classifier_destroy(classifier);
    arowpp_result_destroy(result);

    return 0;
}
