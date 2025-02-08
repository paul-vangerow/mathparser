#include "lexer/lexer.h"
#include "lexer/matcher.h"

void Lexer::addSequence(std::string token, std::string match){
    m_sequences.emplace_back(token, match);
}

std::vector<std::string> Lexer::match_sequence(std::string token_sequence){
    Matcher matcher(m_sequences);
    for (auto c : token_sequence){
        matcher.match_token(c);
    }
    return matcher.get_tokens();
}