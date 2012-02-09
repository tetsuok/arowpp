# To test linking the C++ library to a C program.
# The program should not be installed.
noinst_PROGRAMS = sample_c

sample_c_SOURCES = sample.c
sample_c_LDADD = ../src/libarowpp.la
AM_CPPFLAGS = -W -Wall -I../src
