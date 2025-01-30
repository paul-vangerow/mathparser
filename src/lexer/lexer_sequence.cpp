#include "lexer/lexer_sequence.h"
#include "lexer/node_stack.h"

LexerSequence::LexerSequence(std::string token, std::string match) 
: m_token(token)
{
    NodeStack mainStack;
    for (int i = 0; i < match.size(); i++){
        switch (match[i]){
            case '|': mainStack.addDiverge(); break;
            case '*': mainStack.addOptRecursive(); break;
            case '+': mainStack.addRecursive(); break;
            case '?': mainStack.addOptional(); break;
            case ')': mainStack.addClose(); break;
            case '(': mainStack.addOpen(); break;
            case '[': {
                for (int j = i+1; j < match.size(); j++){
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
    m_global_start->beginSimplifyTransitions();
    m_global_start->printInitial();
    reset();
}

LexerSequence::LexerSequence(LexerSequence & constr){
    m_token = constr.m_token;
    m_global_start = constr.m_global_start;
    reset();
}

MatchResult LexerSequence::match(char c){
    std::queue<LexerNode*> new_iterators;

    MatchResult outval{};
    while(iterators.size()){
        auto front_it = iterators.front();
        iterators.pop();

        // If at least one node has ended (should only ever be one, comunicate that)
        if (front_it->is_end()){
            assert(!outval.is_end);
            outval.is_end = true;
        }

        // Find all matching connections on the iterator.
        front_it->match(c, new_iterators, iterators);
    }
    std::swap(iterators, new_iterators);

    outval.is_empty = (iterators.size() != 0);
    std::cout << "Match result: " << outval.is_empty << " " << outval.is_end << "\n";
    return outval;
}

void LexerSequence::reset(){
    std::queue<LexerNode*> new_queue;
    new_queue.push(m_global_start);
    std::swap( iterators, new_queue );
}

std::string LexerSequence::get_token(){
    return m_token;
}