#pragma once

#include "parser/tokens.h"

struct ParseRule {
    std::string match_string;
};

class ParserSet {
private:
    std::vector<ParseRule> m_rule_set;
    std::string m_token_type;
public:
    ParserSet(std::string token_type) 
    : m_token_type(token_type) {}

    // template <typename T>
    ParserSet& add_rule(std::string match_string){
        ParseRule new_rule{match_string};
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