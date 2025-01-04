#include "lexer/lexer.h"

void Lexer::addSequence(std::string token, std::string match){
    m_sequences.push_back( std::make_unique<LexerSequence>(token, match) );
}