#!/bin/bash

BUILD_DIR=./build/
SRC_DIR=.

rm -r $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
cmake -G Ninja ../ -DCMAKE_BUILD_TYPE=Debug
ninja -v
cd ..