#include "lexer/node_stack.h"

NodeStack::NodeStack(){
    m_nstack.push(std::vector<NodeBlock>());
}

void NodeStack::addNode(NodeBlock newNode){
    auto & top = m_nstack.top();
    if (top.size()) {
        auto & prevElement = top.back();
        if (prevElement.divergent){
            prevElement.entry->addTransition('~', newNode.entry);
            newNode.exit->addTransition('~', prevElement.exit);
            prevElement.divergent = false;
        } else {
            prevElement.exit->addTransition('~', newNode.entry);
            top.push_back(newNode);
        }
    } else {
        top.push_back(newNode);
    }
}

void NodeStack::addOpen(){
    m_nstack.push(std::vector<NodeBlock>());
}

void NodeStack::addClose(){
    addNode(reduce_stack());
}

void NodeStack::addDiverge(){
    auto lastElement = popLast();

    NodeBlock divBlock(true);
    divBlock.entry->addTransition('~', lastElement.entry);
    lastElement.exit->addTransition('~', divBlock.exit);

    addNode(divBlock);
}

NodeBlock NodeStack::reduce_stack(){
    auto & top = m_nstack.top();
    NodeBlock levelBlock( top.front().entry, top.back().exit );
    m_nstack.pop();
    return levelBlock;
}

NodeBlock NodeStack::popLast(){
    auto & stackTop = m_nstack.top();
    auto lastElement = stackTop.back();
    stackTop.pop_back();

    if (stackTop.size()) stackTop.back().exit->removeTransition(lastElement.entry, '~');
    return lastElement;
}

void NodeStack::addOptRecursive(){
    auto lastElement = popLast();

    NodeBlock optBlock(true);
    optBlock.entry->addTransition('~', lastElement.entry);
    optBlock.entry->addTransition('~', optBlock.exit);

    lastElement.exit->addTransition('~', optBlock.exit);
    lastElement.exit->addTransition('~', lastElement.entry);
    
    addNode(optBlock);
}

void NodeStack::addRecursive(){
    auto lastElement = popLast();

    NodeBlock optBlock;
    optBlock.entry->addTransition('~', lastElement.entry);
    lastElement.exit->addTransition('~', optBlock.exit);
    lastElement.exit->addTransition('~', lastElement.entry);
    
    addNode(optBlock);
}

void NodeStack::addOptional(){
    auto lastElement = popLast();

    NodeBlock optBlock(true);
    optBlock.entry->addTransition('~', lastElement.entry);
    optBlock.entry->addTransition('~', optBlock.exit);
    lastElement.exit->addTransition('~', optBlock.exit);
    
    addNode(optBlock);
}

void NodeStack::addSymbol(char s){
    NodeBlock symbolBlock;
    symbolBlock.entry->addTransition(s, symbolBlock.exit);
    addNode(symbolBlock);
}

void NodeStack::addSymbols(std::unordered_set<char> s){
    NodeBlock symbolBlock;
    symbolBlock.entry->addTransitions(s, symbolBlock.exit);
    addNode(symbolBlock);
}

void NodeStack::addSet(std::string setString){
    std::unordered_set<char> characters;
    for (std::size_t i = 0; i < setString.size(); i++){
        if ( i+1 == setString.size() || setString[i+1] != '-' ) characters.insert(setString[i]);
        else {
            for (char c = setString[i]; c <= setString[i+2]; c++) characters.insert(c);
            i += 2;
        }
    }
    addSymbols(characters);
}