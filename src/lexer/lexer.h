#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <memory>

#include "lexer/lexer_sequence.h"

// ---*--- LEXER ---*--- //

class Lexer {
private:
    std::vector<LexerSequence> m_sequences;
public:
    Lexer() = default;

    void addSequence(std::string token, std::string match);

    std::vector<std::string> match_sequence(std::string match_sequence);
};

