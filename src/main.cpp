#include <iostream>
#include "lexer/lexer.h"

int main(int argc, char* argv[]){
    Lexer math_lexer;

    math_lexer.addSequence("NUM", "[0-9]+");
    math_lexer.addSequence("NUM", "[0-9]+(.[0-9]+)?");
    math_lexer.addSequence("INT", "INT");
    math_lexer.addSequence("VAR", "[a-zA-Z]+");
    math_lexer.addSequence("SPACE", " ");
    math_lexer.addSequence("EQ", "=");
    math_lexer.addSequence("DIV", "//");
    math_lexer.addSequence("MUL", "/*");
    math_lexer.addSequence("MINS", "-");
    math_lexer.addSequence("PLUS", "/+");

    auto out = math_lexer.match_sequence("20");

    for (auto item : out){
        std::cout << item << " ";
    }

    return 0;
}