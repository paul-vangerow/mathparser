#pragma once

#include "lexer/lexer_node.h"

// ---*--- LEXER SEQUENCE ---*--- //

class LexerSequence {
private:
    std::string m_token;
    LexerNode* m_global_start;
public:
    LexerSequence(std::string token, std::string match);
};