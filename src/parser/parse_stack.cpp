#include "parser/parse_stack.h"

std::size_t ParseStack::size(){
    return stack.size();
}

void ParseStack::push(std::unique_ptr<Token>&& item){
    stack.push_back(std::move(item));
}

std::string ParseStack::peek_n(std::size_t n){
    std::string out = "";
    for (std::size_t i = size() - n; i < size() - 1; i++){
        out += (stack[i]->type() + " ");
    }
    out += stack.back()->type();
    return out;
}

std::vector<std::unique_ptr<Token>> ParseStack::pop_n(std::size_t n){
    std::vector<std::unique_ptr<Token>> out;
    for (std::size_t i = size() - n; i < size(); i++){
        out.push_back(std::move(stack[i]));
    }
    stack.erase( stack.begin() + (size() - n), stack.end());
    return out;
}

std::unique_ptr<Token>&& ParseStack::get(){
    assert( size() == 1 );
    return std::move(stack.front());
}