#pragma once

#include "parser/tokens.h"
#include <vector>
#include <optional>
#include <lexer/matcher.h>
#include <cassert>

using ConstructionFunction = std::function<std::unique_ptr<Token>(std::vector<std::unique_ptr<Token>>&&)>;

class ParserExpression {
    struct ParseRule {
        std::string match_sequence;
        ConstructionFunction construction_rule;
    };

    std::string m_type;
    std::vector<ParseRule> m_rules;
public:
    ParserExpression(std::string token_type) : m_type(token_type) {}

    template <typename T>
    ParserExpression & add_rule(std::string match_sequence){
        m_rules.push_back({match_sequence, [type_string = m_type](std::vector<std::unique_ptr<Token>>&& in){return std::make_unique<T>(type_string, std::move(in));}});
        return *this;
    }

    std::optional<ConstructionFunction> check_match(std::string match_string){
        for (auto& rule : m_rules){
            if (rule.match_sequence == match_string){
                return rule.construction_rule;
            }
        }
        return {};
    }
};

class ParseStack {
private:
    std::vector<std::unique_ptr<Token>> stack;
public:
    ParseStack() = default;

    std::size_t size(){
        return stack.size();
    }

    void push(std::unique_ptr<Token>&& item){
        stack.push_back(std::move(item));
    }

    std::string peek_n(std::size_t n){
        std::string out = "";
        for (std::size_t i = size() - n; i < size() - 1; i++){
            out += (stack[i]->type() + " ");
        }
        out += stack.back()->type();
        return out;
    }

    std::vector<std::unique_ptr<Token>> pop_n(std::size_t n){
        std::vector<std::unique_ptr<Token>> out;
        for (std::size_t i = size() - n; i < size(); i++){
            out.push_back(std::move(stack[i]));
        }
        stack.erase(stack.begin() + size() - n, stack.end());
        return out;
    }

    std::unique_ptr<Token>&& get(){
        // assert( size() == 1 );
        return std::move(stack.front());
    }
};

class Parser {
    Token root;

    // Store a map of ParserExpressions to make tokens of the given types.
    std::vector<ParserExpression> tokenMap;

    std::optional<ConstructionFunction> seek_rule(std::string match_string){
        for (auto& item : tokenMap){
            std::optional<ConstructionFunction> value = item.check_match(match_string);
            if (value) return value;
        }
        return {};
    }
public:
    Parser() = default;

    // Add a new parser expression
    ParserExpression & add_token(std::string token_type){
        tokenMap.emplace_back(token_type);
        return tokenMap.back();
    }

    std::unique_ptr<Token> parse_stream(std::vector<std::unique_ptr<LexerToken>>&& token_stream){
        ParseStack stack;
        for (auto it = token_stream.begin(); it != token_stream.end(); ++it){
            std::cout << "Trying next\n";
            std::string type = (*it)->type();
            stack.push(std::move(*it));

            // Peek type of next token
            std::string peek_next = "";
            if ( std::next(it) != token_stream.end() ){
                peek_next = (*std::next(it))->type();
            }
            std::cout << "Pushing {"<<type<<"} to stack with peek value {"<<peek_next<<"}\n";
            
            bool reduce = true;

            // If the stack got reduced, run again.
            while(reduce){
                reduce = false;
                for (std::size_t elements = 1; elements <= stack.size(); elements++){
                    std::string match_string = stack.peek_n(elements);

                    auto check_rule = seek_rule(match_string);
                    std::cout << "Check rule on match_string = ("<<match_string<<") has value ("<< check_rule.has_value() <<")\n";

                    // If there is no rule to make the current token sequence, keep looking.
                    if (!check_rule) continue;

                    std::string confirm_string = match_string + " " + peek_next;
                    auto peek_rule = seek_rule(confirm_string);

                    std::cout << "With Peek = ("<<confirm_string<<") has value ("<< peek_rule.has_value() <<")\n";
                    
                    // We have a rule for the current sequence AND there is a rule with the
                    // next token. We are GREEDY so we keep going (get most tokens possible).
                    if (peek_rule) break;

                    // This is the last element with which we have a sequence. Reduce the stack.
                    std::vector<std::unique_ptr<Token>> to_collapse = stack.pop_n(elements);
                    std::cout << "After popping stack has size ("<<stack.size()<<") and to collapse has size "<<to_collapse.size()<<"\n";

                    // Build new token value
                    std::unique_ptr<Token> new_element = (*check_rule)(std::move(to_collapse));
                    std::cout << "Collapsed {"<<elements<<"} elements into one of type {"<<new_element->type()<<"}\n";

                    stack.push(std::move(new_element));
                    reduce = true;
                    break;
                }
            }
        }
        return stack.get();
    }
};