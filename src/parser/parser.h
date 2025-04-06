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

    bool check_rule(std::string target, std::size_t start_idx, std::size_t rule_length, std::vector<std::unique_ptr<Token>>& input_vector){
        assert(start_idx + rule_length - 1 < input_vector.size());
        
        std::string match_check;
        for (std::size_t i = 0; i < rule_length; i++){
            if (!input_vector[start_idx + i]){
                continue; 
            }
            match_check += input_vector[start_idx + i]->get_dtype();
            if (i != rule_length-1) match_check += " ";
        }
        return (match_check == target);
    }

    std::unique_ptr<Token> collapse_token(std::size_t start_idx, std::size_t length, CreationFunction& fn, std::vector<std::unique_ptr<Token>>& input_vector){
        std::vector<std::unique_ptr<Token>> convert;
        for (std::size_t i = 0; i < length; i++){
            convert.push_back(std::move(input_vector[start_idx + i]));
        }
        return fn(std::move(convert));
    }

    void reduce_list(std::size_t start_idx, std::size_t length, std::unique_ptr<Token> new_v, std::vector<std::unique_ptr<Token>>& input_vector){
        input_vector[start_idx] = std::move(new_v);
        if (length > 1){
            input_vector.erase(input_vector.begin() + start_idx + 1, input_vector.begin() + start_idx + length);
        }
    }

    std::unique_ptr<Token> parse(std::vector<std::unique_ptr<Token>>&& in){
        std::stack<std::vector<std::unique_ptr<Token>>> bracket_stack;
        
        // Base stack
        bracket_stack.push(std::vector<std::unique_ptr<Token>>{});

        for (std::size_t i = 0; i < in.size(); i++){
            if (in[i]->get_dtype() == "BRO") bracket_stack.push(std::vector<std::unique_ptr<Token>>{});
            else if (in[i]->get_dtype() == "BRC") {
                auto top_tokens = std::move(bracket_stack.top());
                bracket_stack.pop();

                bracket_stack.top().push_back(parse_token_subset(std::move(top_tokens)));
            } else {
                bracket_stack.top().push_back(std::move(in[i]));
            }
        }

        auto top_tokens = std::move(bracket_stack.top());
        bracket_stack.pop();

        return parse_token_subset(std::move(top_tokens));
    }

    std::unique_ptr<Token> parse_token_subset(std::vector<std::unique_ptr<Token>>&& in){
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
                    
                    // If the match string matches the rule, extract the tokens, build
                    // the new Token and erase the other tokens.
                    if (check_rule(rule_match_string, input_it, rule_match_length, in)){
                        auto new_token = collapse_token(input_it, rule_match_length, rule_make_fn, in);
                        reduce_list(input_it, rule_match_length, std::move(new_token), in);
                        modified = true;
                    }
                }
            }
        }
        assert(in.size() == 1);
        return std::move(in[0]);
    }

    std::size_t get_rule_length(std::string match){
        std::size_t count = 1;
        for (std::size_t i = 0; i < match.size(); i++){
            if (match[i] == ' ') count++;
        }
        return count;
    }
};