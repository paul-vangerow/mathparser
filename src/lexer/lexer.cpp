#include "lexer/lexer.h"

void Lexer::addSequence(std::string token, std::string match){
    m_sequences.addSequence( token, match );
}

std::vector<std::string> Lexer::match_sequence(std::string token_sequence){
    MatchLayerTopWrapper matcher(m_sequences);

    std::cout << "Onto loop\n";

    for (auto token : token_sequence) { 
        std::cout << "Matching " << token << "\n";
        int out = matcher.match(token);
        
        if (out != 0) std::cout << "No good !!! \n";
    }

    return matcher.getTokens();
}