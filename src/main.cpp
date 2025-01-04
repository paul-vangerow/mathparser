#include <iostream>
#include "lexer/lexer.h"

int main(int argc, char* argv[]){
    LexerSequence("something", "a[bc]d");
    return 0;
}