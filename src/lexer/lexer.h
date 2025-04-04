#pragma once

#include "lexer/lexer_sequence.h"
#include "lexer/matcher.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <memory>

// An object for providing an API for all of the lexing logic
class Lexer {
private:
    // The set of ALL sequences in the Lexer.
    std::vector<LexerSequence> m_sequences;
public:
    Lexer() = default;

    // Method to construct a new sequence in place in m_sequences.
    void addSequence(std::string token, std::string match);

    // Method for matching the provided input sequence and getting
    // the resultant list of tokens.
    std::vector<std::unique_ptr<Token>> match_sequence(std::string match_sequence);

    // Helper function for printing a sequence of lexed tokens
    static void print_sequence(std::vector<std::unique_ptr<Token>>& values){
        for (auto& item : values){
            std::cout << item->get_dtype() << " ";
        }
        std::cout << "\n";
    }
};

