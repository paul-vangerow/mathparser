#pragma once
#include "lexer/lexer_node.h"

#include <queue>

// ---*--- LEXER SEQUENCE ---*--- //

class LexerSequence {
private:
    std::string m_token;
    LexerNode* m_global_start;

    std::queue<LexerNode*> iterators;
public:
    LexerSequence(std::string token, std::string match);
    LexerSequence(LexerSequence & constr);
    
    void reset();
};