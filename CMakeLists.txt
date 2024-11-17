cmake_minimum_required(VERSION 3.31)
project(mathparser)

add_executable(mathparser
    ./src/tokenregex/main.cpp
)

include_directories(mathparser
    ./src/
    ./src/tokenregex
)

set(CMAKE_BINARY_DIR "./build/")
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")