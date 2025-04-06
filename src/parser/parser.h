#pragma once

#include "parser/tokens.h"
#include <span>

using CreationFunction = std::function<std::unique_ptr<Token>(std::vector<std::unique_ptr<Token>>&&)>;

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
        auto lambda = [dtype = m_token_type](std::vector<std::unique_ptr<Token>>&& in){
            return std::unique_ptr<Token>(new T(dtype, std::move(in))); 
        };
        m_rule_set.emplace_back(match_string, lambda);
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

    void parse_token_subset(std::vector<std::unique_ptr<Token>>& in){
        // Iterate over each rule
        bool modified = true;
        while (modified){
            modified = false;
            for (auto rule_it = m_rule_set.rbegin(); rule_it != m_rule_set.rend(); ++rule_it){
                auto rule_match_string = rule_it->match_string;
                auto rule_match_length = get_rule_length(rule_match_string);
                auto rule_make_fn = rule_it->make_function; 

                // Iterate over all tokens
                for (std::size_t input_it = 0; input_it + rule_match_length - 1 < in.size(); ++input_it){
                    std::string match_check;
                    // Get the match string of the indexed tokens
                    for (std::size_t i = 0; i < rule_match_length; i++){
                        if (!in[input_it + i]){
                            continue; 
                        }
                        match_check += in[input_it + i]->get_dtype();
                        if (i != rule_match_length-1) match_check += " ";
                    }
                    // If the match string matches the rule, extract the tokens, build
                    // the new Token and erase the other tokens.
                    if (match_check == rule_match_string){
                        std::vector<std::unique_ptr<Token>> convert;
                        for (std::size_t i = 0; i < rule_match_length; i++){
                            convert.push_back(std::move(in[input_it + i]));
                        }
                        auto new_token = rule_make_fn(std::move(convert));
                        in[input_it] = std::move(new_token);
                        if (rule_match_length > 1){
                            in.erase(in.begin() + input_it + 1, in.begin() + input_it + rule_match_length);
                        }
                        modified = true;
                    }
                }
            }
        }
        std::cout << in.size() << "\n";
        in[0]->print(std::cout);
    }

    std::size_t get_rule_length(std::string match){
        std::size_t count = 1;
        for (std::size_t i = 0; i < match.size(); i++){
            if (match[i] == ' ') count++;
        }
        return count;
    }
};