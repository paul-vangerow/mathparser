#pragma once

#include "parser/tokens.h"
#include <span>

using CreationFunction = std::function<std::unique_ptr<Token>&&(std::vector<std::unique_ptr<Token>>&&)>;

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
    std::string m_token_type;
    std::vector<ParseRule>& m_rule_set;
public:
    ParserSet(std::string token_type, std::vector<ParseRule>& ruleset ) 
    : m_token_type(token_type)
    , m_rule_set(ruleset) {}

    template <typename T>
    ParserSet& add_rule(std::string match_string){
        ParseRule new_rule(match_string, [dtype = match_string](std::vector<std::unique_ptr<Token>>&& in){
                                                return std::unique_ptr<Token>(new T(dtype, std::move(in)));
                                            });
        m_rule_set.push_back(new_rule);
        return *this;
    }
};

class Parser {
private:
    std::vector<ParseRule> m_rule_set;
public:
    Parser() = default;

    ParserSet add_set(std::string set_type){
        ParserSet new_set(set_type, m_rule_set);
        return new_set;
    }

    std::unique_ptr<Token>&& parse_token_subset(std::vector<std::unique_ptr<Token>>& in){
        return std::move(std::unique_ptr<Token>(new Token()));
    }

    std::size_t get_rule_length(std::string match){
        std::size_t count = 1;
        for (std::size_t i = 0; i < match.size(); i++){
            if (match[i] == ' ') count++;
        }
        return count;
    }
};