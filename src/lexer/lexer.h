#pragma once

#include "lexer/lexer_sequence.h"
#include "lexer/matcher.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <memory>

// ------------------------ Lexer ------------------------ //
//                                                         //
// An object representing a series of state machines for   //
// a configurable set of regexes. Includes methods to      //
// build the state machines as well as methods to navigate //
// through them.                                           //
//                                                         //
// ------------------------------------------------------- //

class Lexer {
private:
    // The set of ALL sequences in the Lexer.
    std::vector<LexerSequence> m_sequences;
public:
    Lexer() = default;

    // Method to construct a new sequence in place in m_sequences.
    //
    // token: The output token produced by the sequence's
    //        completion.
    // match: The regex the state machine is based on.
    void addSequence(std::string token, std::string match);

    // Method for matching the provided input sequence and getting
    // the resultant list of tokens.
    //
    // match_sequence: The input series of tokens to match using the
    //         provided regexes.
    //
    // return: A vector of produced tokens after matching all input
    //        tokens (The Lexing result).
    std::vector<std::unique_ptr<LexerToken>> match_sequence(std::string match_sequence);
};

