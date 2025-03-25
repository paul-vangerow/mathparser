#pragma once

#include "parser/tokens.h"

using CreationFunction = std::function<std::unique_ptr<Token>&&(std::string, std::vector<std::unique_ptr<Token>>&&)>;

struct ParseRule {
    std::string match_string;
    CreationFunction make_function;

    ParseRule(
        std::string _match_string_, 
        CreationFunction _make_function_) 
    : match_string(_match_string_)
    , make_function(_make_function_)
    {}
};

class ParserSet {
private:
    std::vector<ParseRule> m_rule_set;
    std::string m_token_type;
public:
    ParserSet(std::string token_type) 
    : m_token_type(token_type) {}

    template <typename T>
    ParserSet& add_rule(std::string match_string){
        ParseRule new_rule(match_string, [](std::string dtype, 
                                            std::vector<std::unique_ptr<Token>>&& in){
                                                return std::unique_ptr<Token>(new T(dtype, std::move(in)));
                                            });
        m_rule_set.push_back(new_rule);
        return *this;
    }
};

class Parser {
private:
    std::vector<ParserSet> m_rule_sets;
public:
    Parser() = default;

    ParserSet& add_set(std::string set_type){
        m_rule_sets.emplace_back(set_type);
        return m_rule_sets.back();
    }
};