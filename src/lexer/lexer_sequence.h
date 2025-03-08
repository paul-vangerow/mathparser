#pragma once

#include "lexer/lexer_node.h"
#include "lexer/node_stack.h"

#include <queue>

// -------------------- LexerSequence -------------------- //
//                                                         //
// An object representing a state machine for a single     //
// Regex, producing its specified token when fully         //
// matched.                                                //
//                                                         //
// ------------------------------------------------------- //

class LexerSequence {
private:
    // The token produced when this sequence is matched
    std::string m_token;

    // The entry into this sequence. Iterators for the
    // sequence will always start at this node.
    LexerNode* m_global_start;

    // A list of all current iterators in the NFA (representing
    // all possible current states the machine could be in.
    std::queue<LexerNode*> m_iterators;
public:
    // Constructor for a Lexer Sequence. This constructs the
    // series of States and links them together based on the
    // provided regex.
    //
    // token: The output token of the sequence.
    // match: The regex representing the sequence.
    LexerSequence(std::string token, std::string match);

    // A getter for the token.
    //
    // return: value of m_token
    std::string get_token();
    
    // A method to advance all current states given the next
    // character in the sequence and return the properties of
    // the resultant 'next' states.
    //
    // c: The next token in the input sequence
    //
    // return: The result state of the match operation
    //         indicating if any matches took place and if the
    //         sequence was successfully fully matched by any
    //         of the iterators.
    MatchResult match_token(char c);

    // A method to reset the iterator states (reset the whole
    // sequence back to its initial state.
    void reset_iterators();

    // A method to print all current iterators (And what states
    // they're occupying.
    void print_iterators();
};