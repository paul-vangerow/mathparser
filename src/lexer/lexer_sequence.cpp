#include "lexer/lexer_sequence.h"
#include "lexer/node_stack.h"

LexerSequence::LexerSequence(std::string token, std::string match) 
: m_token(token)
{
    NodeStack mainStack;
    for (std::size_t i = 0; i < match.size(); i++){
        switch (match[i]){
            case '|': mainStack.addDiverge(); break;
            case '*': mainStack.addOptRecursive(); break;
            case '+': mainStack.addRecursive(); break;
            case '?': mainStack.addOptional(); break;
            case ')': mainStack.addClose(); break;
            case '(': mainStack.addOpen(); break;
            case '[': {
                for (std::size_t j = i+1; j < match.size(); j++){
                    if ( match[j] == ']' ){
                        mainStack.addSet( match.substr(i+1, j-i-1) );
                        i = j;
                        break;
                    }
                }
            } break;
            case '/': i++;
            default: mainStack.addSymbol(match[i]); break;
        }
    }
    m_global_start = mainStack.reduce_stack().entry;
    m_global_start->printInitial();
    reset_iterators();
}

std::string LexerSequence::get_token(){
    return m_token;
}

MatchResult LexerSequence::match_token(char c){
    std::queue<LexerNode*> new_iterators;

    MatchResult result{.is_end=false, .is_empty=true};
    while (m_iterators.size()){
        auto front_it = m_iterators.front();
        m_iterators.pop();

        // Get all epsilon connections from the node and add them to the
        // iterators queue.
        front_it->handle_preiterators(m_iterators);
        
        for (auto item : front_it->match(c)){
            item->handle_postiterators(result, new_iterators);
        }
    }
    std::swap(m_iterators, new_iterators);
    return result;
}
// Can likely be removed.
void LexerSequence::reset_iterators(){
    std::queue<LexerNode*> tmp;
    tmp.push(m_global_start);
    std::swap(m_iterators, tmp);
}

void LexerSequence::print_iterators(){
    std::queue<LexerNode*> store;
    while (m_iterators.size()){
        auto front = m_iterators.front();
        m_iterators.pop();
        front->printSingular();
        store.push(front);
    }
    std::swap(store, m_iterators);
}