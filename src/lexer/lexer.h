#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

// ---*--- LEXER NODE ---*--- //

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

    void addTransitions(std::unordered_set<char> values, LexerNode* target){
        for (auto item : values) addTransition(item, target);
    }

    bool removeTransition(LexerNode* node, char key){
        auto it = m_transitions.find(key);
        if (it == m_transitions.end()) return false;
        auto & vec = it->second;
        for (auto vecIt = vec.begin(); vecIt != vec.end(); ++vecIt){
            if (*vecIt == node) {
                vec.erase(vecIt);
                if (vec.size() == 0) m_transitions.erase(it);
                return true;
            }
        }
        return false;
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
    , exit(new LexerNode(true))
    , divergent(div){}

    NodeBlock(LexerNode* _entry, LexerNode* _exit)
    : entry(_entry)
    , exit(_exit)
    , divergent(false){}
};

// ---*--- NODE STACK ---*--- //

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
        if (top.size()) {
            auto & prevElement = top.back();
            if (prevElement.divergent){
                prevElement.entry->addTransition('.', newNode.entry);
                newNode.exit->addTransition('.', prevElement.exit);
                prevElement.divergent = false;
            } else {
                prevElement.exit->addTransition('.', newNode.entry);
                top.push_back(newNode);
            }
        } else {
            top.push_back(newNode);
        }
    }

    void addOpen(){
        nstack.push(std::vector<NodeBlock>());
    }

    void addClose(){
        addNode(reduce_stack());
    }

    void addDiverge(){
        auto lastElement = popLast();

        NodeBlock divBlock(true);
        divBlock.entry->addTransition('.', lastElement.entry);
        lastElement.exit->addTransition('.', divBlock.exit);

        addNode(divBlock);
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

    NodeBlock popLast(){
        auto & stackTop = nstack.top();
        auto lastElement = stackTop.back();
        stackTop.pop_back();

        if (stackTop.size()) stackTop.back().exit->removeTransition(lastElement.entry, '.');
        return lastElement;
    }

    void addOptRecursive(){
        auto lastElement = popLast();

        NodeBlock optBlock;
        optBlock.entry->addTransition('.', lastElement.entry);
        optBlock.entry->addTransition('.', optBlock.exit);

        lastElement.exit->addTransition('.', optBlock.exit);
        lastElement.exit->addTransition('.', lastElement.entry);
        
        addNode(optBlock);
    }

    void addRecursive(){
        auto lastElement = popLast();

        NodeBlock optBlock;
        optBlock.entry->addTransition('.', lastElement.entry);
        lastElement.exit->addTransition('.', optBlock.exit);
        lastElement.exit->addTransition('.', lastElement.entry);
        
        addNode(optBlock);
    }

    void addOptional(){
        auto lastElement = popLast();

        NodeBlock optBlock;
        optBlock.entry->addTransition('.', lastElement.entry);
        optBlock.entry->addTransition('.', optBlock.exit);
        lastElement.exit->addTransition('.', optBlock.exit);
        
        addNode(optBlock);
    }

    void addSymbol(char s){
        NodeBlock symbolBlock;
        symbolBlock.entry->addTransition(s, symbolBlock.exit);
        addNode(symbolBlock);
    }

    void addSet( std::string setString ){
        std::unordered_set<char> characters;
        for (int i = 0; i < setString.size(); i++){
            if ( i+1 == setString.size() || setString[i+1] != '-' ) characters.insert(setString[i]);
            else {
                for (char c = setString[i]; c <= setString[i+2]; c++) characters.insert(c);
                i += 2;
            }
        }
        addSymbols(characters);
    }

    void addSymbols(std::unordered_set<char> s){
        NodeBlock symbolBlock;
        symbolBlock.entry->addTransitions(s, symbolBlock.exit);
        addNode(symbolBlock);
    }

    void printAllNodes(){
        for (auto item : allNodes){
            item->printNode();
        }
    }
};

// ---*--- LEXER SEQUENCE ---*--- //

class LexerSequence {
private:
    std::string m_token;
    LexerNode* m_global_start;
public:
    LexerSequence(std::string token, std::string match);
};

// ---*--- LEXER ---*--- //

class Lexer {
private:
    std::vector<std::unique_ptr<LexerSequence>> m_sequences;
public:
    Lexer() = default;

    void addSequence(std::string token, std::string match);
};

