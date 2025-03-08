#pragma once

#include "lexer/lexer_node.h"

#include <stack>
#include <iostream>
#include <unordered_set>

// ---------------------- NodeStack ---------------------- //
//                                                         //
// An object to help with the construction of the NFA via  //
// Thompson's construction. A stack of sequences where     //
// stack item contains a sequence of NodeBlocks and each   //
// stack layer represents a self contained context (similar//
// to brackets in mathematical equations).                 //
//                                                         //
// ------------------------------------------------------- //

class NodeStack {
private:
    // The stack data structure
    std::stack<std::vector<NodeBlock>> m_nstack;
public:
    NodeStack();
    ~NodeStack() = default;

    // Add a new node to the stack. This will typically
    // simply append to the current top list but if the
    // top is marked divergent it will add it within the
    // top-last node.
    //
    // newNode: The NodeBlock to add to the stack.
    void addNode(NodeBlock newNode);

    // Add an additional layer to the NodeStack.
    void addOpen();

    // Convert the top layer into a single NodeBlock
    // and pop it from the stack, adding it to the
    // next layer down.
    void addClose();

    // Make the previous node (top-last) a divergent
    // node (wrap it in another node and make the 
    // next added node be inserted INSIDE instead of
    // being appended to the back)
    void addDiverge();

    // Add the OptionalRecursive property to the
    // previous node (regex * property).
    void addOptRecursive();

    // Add the recursive property to the previous
    // node (regex + property)
    void addRecursive();

    // Add the optional property to the previous
    // node (regex ? property)
    void addOptional();

    // Add a new node with a symbol transition going
    // to it from the previous node.
    //
    // s: The character necessary for the transition.
    void addSymbol(char s);

    // AddSymbol but for a set of characters
    //
    // s: The Set of characters to consider.
    void addSymbols(std::unordered_set<char> s);

    // Create a set of characters based on the
    // regex syntax string. And call 'addSymbols'
    //
    // setString: The regex style set definition.
    void addSet(std::string setString);

    // Pop the top layer of the stack and wrap it
    // in a node block.
    //
    // return: The wrapper NodeBlock.
    NodeBlock reduce_stack();

    // Remove the top-last element from the stack
    // and return it (this can only be done for
    // elements linked to by epsilon transitions)
    //
    // return: The popped element.
    NodeBlock popLast();
};