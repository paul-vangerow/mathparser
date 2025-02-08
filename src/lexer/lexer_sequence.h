#pragma once
#include "lexer/lexer_node.h"

#include <queue>

// ---*--- LEXER SEQUENCE ---*--- //

class LexerSequence {
private:
    std::string m_token;
    LexerNode* m_global_start;

    std::queue<LexerNode*> m_iterators;
public:
    LexerSequence(std::string token, std::string match);

    std::string get_token();

    MatchResult match_token(char c);
    void reset_iterators();
    void print_iterators();
};