#!/bin/sh -f
BLD_DIR=../src
TRAINER=$BLD_DIR/arow_learn
TESTER=$BLD_DIR/arow_test

for f in $TRAINER $TESTER; do
    if ! [ -x $f ]; then
        echo "ERROR: $f not found."
        echo "You should complile programs first."
        exit 1
    fi
done

TRAIN_FILE=train.dat
TEST_FILE=test.dat
MODEL=model

$TRAINER -r 2.6 $TRAIN_FILE $MODEL && echo "" && \
    $TESTER $TEST_FILE $MODEL && \
    rm $MODEL
