#pragma once

#include "parser/tokens.h"

#include <vector>

// ---------------------- ParseStack --------------------- //
//                                                         //
// An object to help with the implementation of a          //
// look-ahead shift-reduce parser. Keeps track of all      //
// current tokens and helps with adding new items,         //
// matching them with grammar rules and reducing them into //
// the appropriate token. At the end should result in only //
// a single Token.                                         //
//                                                         //
// ------------------------------------------------------- //

class ParseStack {
private:
    // The stack of all added Tokens. 
    std::vector<std::unique_ptr<Token>> stack;
public:
    ParseStack() = default;

    // Method to get the current size of the stack
    //
    // return: stack.size()
    std::size_t size();

    // Method to add a new token to the stack
    //
    // item: The Token to add to the stack.
    void push(std::unique_ptr<Token>&& item);

    // A method to get a space separated string of
    // token types for the top n elements in the
    // stack.
    //
    // n: The number of elements to peek at
    //
    // return: The string of all their types
    //         separated by spaces.
    std::string peek_n(std::size_t n);

    // A method to pop the top n elements of the stack
    // and return them.
    //
    // n: The number of elements to pop.
    //
    // return: The list of all the popped tokens.
    std::vector<std::unique_ptr<Token>> pop_n(std::size_t n);

    // A method to get the top token of the stack. This
    // is to be used after all shift-reduce steps and
    // should return the root of the AST.
    //
    // return: The root of the AST.
    std::unique_ptr<Token>&& get();
};