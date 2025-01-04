#include "lexer/lexer_node.h"

LexerNode::LexerNode(bool is_end) : m_is_end(is_end)
{
    static std::size_t nodeNumber = 0;
    m_node_number = nodeNumber++;
}

void LexerNode::addTransition(char value, LexerNode* target){
    m_is_end = false;
    auto it = m_transitions.find(value);
    if (it == m_transitions.end()) m_transitions.insert({value, std::vector<LexerNode*>{target}});
    else it->second.push_back(target);
}

void LexerNode::addTransitions(std::unordered_set<char> values, LexerNode* target){
    for (auto item : values) addTransition(item, target);
}

bool LexerNode::removeTransition(LexerNode* node, char key){
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

void LexerNode::printInitial(){
    std::unordered_set<int> visited;
    std::cout << "Start: ";
    printNode(visited);
    std::cout << "\n";
    std::cout << "------- \n";
}

void LexerNode::printNode( std::unordered_set<int>& visited){
    visited.insert( m_node_number );
    std::cout << "("<<m_node_number<<"): ";

    std::vector<LexerNode*> connections;
    for (auto i : m_transitions){
        std::cout << i.first << ": ";
        for (auto j : i.second) {
            std::cout << j->m_node_number << " ";
            connections.push_back(j);
        }
    }
    std::cout << "\n";
    for (auto item : connections){
        if (visited.find(item->m_node_number) != visited.end()) continue;
        item->printNode(visited);
    }
}