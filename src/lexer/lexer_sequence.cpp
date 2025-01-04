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
}