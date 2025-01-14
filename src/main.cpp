#include <iostream>
#include "lexer/lexer.h"

int main(int argc, char* argv[]){
    Lexer math_lexer;
    math_lexer.addSequence("something", "abcd");

    std::string input_sequence = "abcd";
    // auto output = math_lexer.match_sequence(input_sequence);

    // for (auto item : output) std::cout << item << " ";
    // std::cout << "\n";

    return 0;
}