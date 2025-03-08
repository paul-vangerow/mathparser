#include "lexer/lexer.h"

void Lexer::addSequence(std::string token, std::string match){
    m_sequences.emplace_back(token, match);
}

std::vector<std::unique_ptr<LexerToken>> Lexer::match_sequence(std::string token_sequence){
    Matcher matcher(m_sequences);
    for (auto c : token_sequence){
        matcher.match_token(c);
    }
    return matcher.get_tokens();
}