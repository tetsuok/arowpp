lib_LTLIBRARIES = libarowpp.la

libarowpp_la_SOURCES = \
	classifier.cc \
	feature.cc \
	libarowpp.cc \
	model_reader.cc \
	model_writer.cc \
	options.cc \
	result.cc \
	tokenizer.cc

include_HEADERS = arowpp.h
bin_PROGRAMS = arow_learn arow_test

arow_learn_SOURCES = arow_learn.cc
arow_learn_LDADD = libarowpp.la

arow_test_SOURCES = arow_test.cc
arow_test_LDADD = libarowpp.la

# unit tests using Google Test.
if HAVE_GTEST
check_PROGRAMS = \
	classifier_test \
	feature_test \
	model_reader_test \
	result_test \
	tokenizer_test

# unit tests you want to run when people type 'make check'.
TESTHARNESS = testharness.cc

TESTS = \
	classifier_test \
	feature_test \
	model_reader_test \
	result_test \
	tokenizer_test

classifier_test_SOURCES = classifier_test.cc $(TESTHARNESS)
classifier_test_LDADD = $(GTEST_LDFLAGS) $(GTEST_LIBS) libarowpp.la

feature_test_SOURCES = feature_test.cc $(TESTHARNESS)
feature_test_LDADD = $(GTEST_LDFLAGS) $(GTEST_LIBS) libarowpp.la

model_reader_test_SOURCES = model_reader_test.cc $(TESTHARNESS)
model_reader_test_LDADD = $(GTEST_LDFLAGS) $(GTEST_LIBS) libarowpp.la

result_test_SOURCES = result_test.cc $(TESTHARNESS)
result_test_LDADD = $(GTEST_LDFLAGS) $(GTEST_LIBS) libarowpp.la

tokenizer_test_SOURCES = tokenizer_test.cc $(TESTHARNESS)
tokenizer_test_LDADD = $(GTEST_LDFLAGS) $(GTEST_LIBS) libarowpp.la
endif
