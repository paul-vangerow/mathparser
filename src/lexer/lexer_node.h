#pragma once

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>

// ---*--- LEXER NODE ---*--- //

class LexerNode {
private:
    std::unordered_map<char, std::vector<LexerNode*>> m_transitions;
    bool m_is_end;
    bool m_is_divergence_target;
    std::size_t m_node_number;
public:
    LexerNode(bool is_end = false, bool divergence_target = false);

    // Modify Transititions
    void addTransition(char value, LexerNode* target);
    void addTransitions(std::unordered_set<char> values, LexerNode* target);
    bool removeTransition(LexerNode* node, char key);

    void beginSimplifyTransitions();
    void simplifyTransitions(std::unordered_set<int>& visited);

    // Print Nodes
    void printInitial();
    void printNode( std::unordered_set<int>& visited);
};

// ---*--- NODE BLOCK ---*--- //

class NodeBlock {
public:
    LexerNode* entry;
    LexerNode* exit;
    bool divergent;

    NodeBlock()
    : entry(new LexerNode())
    , exit(new LexerNode(true))
    , divergent(false){}

    NodeBlock(bool div)
    : entry(new LexerNode())
    , exit(new LexerNode(true, true))
    , divergent(div){}

    NodeBlock(LexerNode* _entry, LexerNode* _exit)
    : entry(_entry)
    , exit(_exit)
    , divergent(false){}
};