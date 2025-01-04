cmake_minimum_required(VERSION 3.31)
project(mathparser)

add_executable(mathparser
    ./src/main.cpp
    ./src/lexer/lexer.cpp
    ./src/lexer/lexer_node.cpp
    ./src/lexer/node_stack.cpp
)

include_directories(mathparser
    ./src/
    ./src/lexer
)

set(CMAKE_BINARY_DIR "./build/")
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")