#include "lexer/lexer_node.h"

LexerNode::LexerNode(bool is_end) 
: m_is_end(is_end)
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

bool LexerNode::is_end(){
    return m_is_end;
}

std::vector<LexerNode*> LexerNode::match(char c){
    auto it = m_transitions.find(c);
    if (it != m_transitions.end()) return it->second;
    else return std::vector<LexerNode*>{};
}

void LexerNode::handle_postiterators(MatchResult & result, std::queue<LexerNode*>& new_iterators){
    result.is_end |= m_is_end;

    auto epsilon_transitions = m_transitions.find('~');
    bool empty = ( (m_transitions.size() == 0) || (m_transitions.size() == 1 && epsilon_transitions != m_transitions.end()) );

    result.is_empty &= empty;
    if (!empty){
        new_iterators.push(this);
    }
    if (epsilon_transitions != m_transitions.end()){
        for (auto item : epsilon_transitions->second){
            item->handle_postiterators(result, new_iterators);
        }
    }
}

// This should usually not have to be invoked.
void LexerNode::handle_preiterators(std::queue<LexerNode*>& old_iterators){
    auto it = m_transitions.find('~');
    if (it != m_transitions.end()){
        for (auto item : it->second){
            old_iterators.push(item);
        }
    }
}

void LexerNode::printInitial(){
    std::unordered_set<int> visited;
    std::cout << "Start: ";
    printNode(visited);
    std::cout << "\n";
    std::cout << "------- \n";
}

void LexerNode::printSingular(){
    std::cout << "("<<m_node_number<<") ";
}

void LexerNode::printNode( std::unordered_set<int>& visited){
    visited.insert( m_node_number );
    std::cout << "("<<m_node_number<<") ";

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