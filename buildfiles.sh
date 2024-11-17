#!/bin/bash

BUILD_DIR=./build/
SRC_DIR=.

rm -r $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
cmake -G Ninja ../
ninja -v
cd ..