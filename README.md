arowpp
======

Introduction
------------

arowpp (AROW++) is an simple and efficient implementation of Adaptive
Regularization of Weights online learning algorithm for binary
classification. AROW is efficient for learning tasks such as natural
language processing tasks that the data is high-dimensional, extremely
sparse. AROW is an extension of confidence weighted (CW) [Dredze+
2008] algorithm that can achieve the good performance with a few
iterations.

Features
--------

- Written in C++ with STL
- Robustness in the case of non-separable data
- Uses sparse vector representation
- Can handle several hundred-thousands of training examples and feature dimension


Build Instructions
------------------

#### Software Requirements

We have tested our code on Ubuntu Linux 10.04 (x86_64) and OS X 10.7.3 with the following packaged installed.

- GNU C++ compiler (developed with g++ 4.4.3, Apple's g++ 4.2.1) and Apple's clang 3.0.
- [Bazel](http://bazel.io/) for building library and binaries.
- Google C++ Testing Framework (Optional. This requires only for unit tests.)

Note: As it turns out, we decided to use Autotools instead of using
waf since the release of 0.1. The resulting package doesn't depend on
python anymore.

Installation
------------

    $ git clone https://github.com/tetsuok/arowpp.git
    $ cd arowpp
    $ bazel build //:arow_learn //:arow_test

Note that If you want to run unit tests, run `bazel test //:arow_unittest`.

Usage
-----

#### Data format

AROW++ accepts the same representation of training data as [SVMlight](http://svmlight.joachims.org/)
uses. This format has potential to handle large sparse feature
vectors. The format of training and test data file is:

(BNF-like representation)

    <class> .=. +1 | -1
    <feature> .=. integer (>=1)
    <value> .=. real
    <line> .=. <class> <feature>:<value><feature>:<value> ... <feature>:<value>

Here’s an example of such a file:

    +1 201:1 3148:1 3983:1 4882:1
    -1 874:1 3652:1 3963:1 6179:1
    -1 1331:1 3084:1 3957:1 4514:1
    -1 643:1 1870:1 3957:1 4367:1


Training
--------

Use `arow_learn` command.

    $ arow_learn -i int -r float -s train_file model

where `train_file` is the training data you need to prepare in
advance. `arow_learn` will generates the trained model file in` model`.

There are 3 major parameters to control the training condition:

- -i: Number of iteration at training. Default setting is 1, but
       the AROW algorithm can achieve the good performance with a few
       iteration.

- -r: Regularization parameter (Default 0.1). You can optimize
       this parameter depending on the data.

- -s: Shuffle training examples if this option is set (Default not
       shuffle). The AROW algorithm depends on the order of training
       data.

`arow_learn` outputs the following information:

    $ arow_learn train1 model1
    Number of features: 1355191
    Number of examples: 15000
    Number of updates:  9052
    Done.
    time: 3.778 sec.


Testing
-------

Use `arow_test` command.

    $ arow_test test_file model

where `test_file` is the test data, and `model` is the trained model
file `arow_learn` generated.

Here is a typical output of `arow_test`:

    $ arow_test test1 model1
    Accuracy 96.537% (4823/4996)
    (Answer, Predict): (t,p):2480 (t,n):80 (f,p):93 (f,n):2343
    time: 1.097 sec.


References
----------

- [Crammer+ 2009] K. Crammer, A. Kulesza, and M. Dredze, Adaptive Regularization of Weight Vectors. In Advances in Neural Information Processing Systems (NIPS), 2009.
- [Dredze+ 2008] M. Dredze, K. Crammer, and F. Pereira, Conﬁdence-weighted linear classiﬁcation. In Proc. of the 25th international conference on Machine Learning (ICML), 2008.


Acknowledgements
----------------

This program is originally based on the implementation in Java written
by Masashi Tsubosaka. Thanks for Daisuke Okanohara for his
[oll](https://code.google.com/p/oll/) tool that has been a good
reference for the development of AROW++. I would also like to thank
Taku Kudo for learning how to design C/C++ APIs from his software:
[MeCab](https://code.google.com/p/mecab/) and
[zinnia](http://zinnia.sourceforge.net/).
