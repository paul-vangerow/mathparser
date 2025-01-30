#include "lexer/lexer_node.h"

LexerNode::LexerNode(bool is_end, bool divergence_target) 
: m_is_end(is_end)
, m_is_divergence_target(divergence_target)
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

void LexerNode::match(char c, std::queue<LexerNode*>& target, std::queue<LexerNode*>& current){
    auto epsilon_transitions = m_transitions.find('.');

    // Add All Epsilon transitions to those currently being considered. This ensures we 
    // can take into account epsilon activated end nodes.
    if (epsilon_transitions != m_transitions.end()){
        for (auto skipto : epsilon_transitions->second ) current.push(skipto);
    }

    std::unordered_set<LexerNode*> targetAdditions;
    auto char_transitions = m_transitions.find(c);

    // Get all connections based on the token provided
    for (auto next : char_transitions->second ) targetAdditions.insert(next);

    // Ensure we only add unique items (To prevent duplicated navigations)
    for (auto item : targetAdditions) target.push(item);
}

bool LexerNode::is_end(){
    return m_is_end;
}

void LexerNode::beginSimplifyTransitions(){
    std::unordered_set<int> visited;
    simplifyTransitions(visited);
}

void LexerNode::simplifyTransitions(std::unordered_set<int>& visited){
    visited.insert(m_node_number);
    if (m_transitions.size() == 1){
        auto it = m_transitions.find('.');
        if (it != m_transitions.end() && it->second.size() == 1 && !it->second.front()->m_is_divergence_target){
            // Remove transition and merge nodes.
            LexerNode* target = it->second.front();
            m_transitions = target->m_transitions;
            m_node_number = target->m_node_number;
            m_is_end = target->m_is_end;
            delete target;
            // std::cout << "Transition ("<<m_node_number<<") -> ("<<target->m_node_number<<") is redundant\n";
        }
    }
    for (auto i : m_transitions) for (auto j : i.second){
        if ( visited.find(j->m_node_number) == visited.end() ) j->simplifyTransitions(visited);
    }
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