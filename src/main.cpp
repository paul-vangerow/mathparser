#include <iostream>
#include "lexer/lexer.h"

int main(int argc, char* argv[]){
    LexerSequence("something", "d[abc]e");
    return 0;
}