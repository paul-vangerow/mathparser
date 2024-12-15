#include "lexer/lexer.h"

LexerSequence::LexerSequence(std::string token, std::string match) 
: m_token(token)
{
    NodeStack mainStack;
    for (int i = 0; i < match.size(); i++){
        switch (match[i]){
            case '[': {
                for (int j = i+1; j < match.size(); j++){
                    if ( match[j] == ']' ){
                        mainStack.addSet( match.substr(i+1, j-i-1) );
                        i = j;
                        break;
                    }
                }
            } break;
            default: {
                mainStack.addSymbol(match[i]);
            } break;
        }
    }
    m_global_start = mainStack.finish().entry;
    std::cout << "Start:\n";
    m_global_start->printNode();
    std::cout << "\n";
    mainStack.printAllNodes();
    std::cout << "------- \n";
}

void Lexer::addSequence(std::string token, std::string match){
    m_sequences.push_back( std::make_unique<LexerSequence>(token, match) );
}