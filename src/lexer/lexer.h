#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

class LexerNode {
private:
    std::unordered_map<char, std::vector<LexerNode*>> m_transitions;
    bool m_is_end;
    std::size_t nodeNum;
public:
    LexerNode(bool is_end = false) 
    : m_is_end(is_end)
    {
        static std::size_t nodeNumber = 0;
        nodeNum = nodeNumber++;
    }

    bool isEnd() { return m_is_end; };

    void addTransition(char value, LexerNode* target){
        m_is_end = false;
        auto it = m_transitions.find(value);
        if (it == m_transitions.end()) m_transitions.insert({value, std::vector<LexerNode*>{target}});
        else it->second.push_back(target);
    }

    void printNode(){
        std::cout << "("<<nodeNum<<"): ";
        for (auto i : m_transitions){
            std::cout << i.first << ": ";
            for (auto j : i.second) std::cout << j->nodeNum << " ";
        }
        std::cout << "\n";
    }
};

class NodeBlock {
public:
    LexerNode* entry;
    LexerNode* exit;

    NodeBlock(){
        exit = new LexerNode(true);
        entry = new LexerNode();
    }

    NodeBlock(LexerNode* _entry, LexerNode* _exit)
    : entry(_entry)
    , exit(_exit) {}
};

class NodeStack {
private:
    std::stack<std::vector<NodeBlock>> nstack;
    std::unordered_set<LexerNode*> allNodes;
public:
    NodeStack(){
        nstack.push(std::vector<NodeBlock>());
    }

    void addNode(NodeBlock newNode){
        allNodes.insert(newNode.entry);
        allNodes.insert(newNode.exit);
        auto & top = nstack.top();
        if (top.size()) top.back().exit->addTransition('.', newNode.entry);
        top.push_back(newNode);
    }

    void addOpen(){
        nstack.push(std::vector<NodeBlock>());
    }

    void addClose(){
        addNode(reduce_stack());
    }

    NodeBlock reduce_stack(){
        auto & top = nstack.top();
        NodeBlock levelBlock( top.front().entry, top.back().exit );
        nstack.pop();
        return levelBlock;
    }

    NodeBlock finish(){
        NodeBlock newNode = reduce_stack();
        allNodes.insert(newNode.entry);
        allNodes.insert(newNode.exit);
        return newNode;
    }

    void addSymbol(char s){
        NodeBlock symbolBlock;
        symbolBlock.entry->addTransition(s, symbolBlock.exit);
        addNode(symbolBlock);
    }

    void printAllNodes(){
        for (auto item : allNodes){
            item->printNode();
        }
    }
};

class LexerSequence {
private:
    std::string m_token;
    LexerNode* m_global_start;
public:
    LexerSequence(std::string token, std::string match);
};

class Lexer {
private:
    std::vector<std::unique_ptr<LexerSequence>> m_sequences;
public:
    Lexer() = default;

    void addSequence(std::string token, std::string match);
};

