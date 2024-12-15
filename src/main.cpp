#include <iostream>
#include "lexer/lexer.h"

int main(int argc, char* argv[]){
    LexerSequence("something", "ab(cde)");
    return 0;
}