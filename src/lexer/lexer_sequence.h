#pragma once
#include "lexer/lexer_node.h"

#include <queue>

// ---*--- LEXER SEQUENCE ---*--- //

struct MatchResult {
    bool is_end;
    bool is_empty;
};

class LexerSequence {
private:
    std::string m_token;
    LexerNode* m_global_start;
public:
    LexerSequence(std::string token, std::string match);

    std::string get_token();
};