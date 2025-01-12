#include "lexer/lexer.h"

void Lexer::addSequence(std::string token, std::string match){
    m_sequences->addSequence( token, match );
}

std::vector<std::string> Lexer::match_sequence(std::string token_sequence){
    MatchLayerTopWrapper matcher(m_sequences);

    return matcher.getTokens();
}