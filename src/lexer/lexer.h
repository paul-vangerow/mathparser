#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "lexer/lexer_sequence.h"

// ---*--- LEXER ---*--- //

class Lexer {
private:
    std::vector<std::unique_ptr<LexerSequence>> m_sequences;
public:
    Lexer() = default;

    void addSequence(std::string token, std::string match);
};

