#include <iostream>
#include "tokenregex/tokenregex.hpp"

int main(int argc, char* argv[]){
    std::string basic_sequence = "(abc|de)f";
    
    auto sequenceConstruct = convertBasicTokenSequence(basic_sequence);
    std::cout << sequenceConstruct;

    return 0;
}