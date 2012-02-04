#!/bin/sh
PROGRAM=./sample_c

if ! [ -x $PROGRAM ]; then
    echo "ERROR: $PROGRAM not found."
    echo "You should complile programs first."
    exit 1
fi

TRAIN_FILE=train.dat
TEST_FILE=test.dat

$PROGRAM $TRAIN_FILE $TEST_FILE && rm model
