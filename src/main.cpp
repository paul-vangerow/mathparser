#include <iostream>
#include "lexer/lexer.h"

int main(int argc, char* argv[]){
    Lexer math_lexer;
    math_lexer.addSequence("something", "abcd");

    return 0;
}