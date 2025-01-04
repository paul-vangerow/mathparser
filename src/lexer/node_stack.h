#pragma once

#include <stack>
#include <iostream>
#include <unordered_set>
#include "lexer/lexer_node.h"

// ---*--- NODE STACK ---*--- //

class NodeStack {
private:
    std::stack<std::vector<NodeBlock>> m_nstack;
public:
    NodeStack();

    void addNode(NodeBlock newNode);
    void addOpen();
    void addClose();
    void addDiverge();
    void addOptRecursive();
    void addRecursive();
    void addOptional();
    void addSymbol(char s);
    void addSymbols(std::unordered_set<char> s);
    void addSet(std::string setString);

    NodeBlock reduce_stack();
    NodeBlock popLast();
};