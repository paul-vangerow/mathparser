#pragma once

#include "parser/tokens.h"

class ParserExpression {
    struct ParseRule {
        std::string match_sequence;
        std::function<LexerToken(std::vector<LexerToken>)> construction_rule;
    };

    std::string m_type;
    std::vector<ParseRule> m_rules;
public:
    ParserExpression(std::string token_type) : m_type(token_type) {}

    template <typename T>
    ParserExpression & add_rule(std::string match_sequence){
        m_rules.push_back({match_sequence, [type_string = m_type](std::vector<LexerToken> in){return T(type_string, in);}});
        return *this;
    }

    std::optional<LexerToken> match_token_list(std::string bare_token_list, std::vector<LexerToken> list){
        for (auto rule : m_rules){
            if (rule.match_sequence == bare_token_list) return rule.construction_rule(list);
        }
        return {};
    }
};

class Parser {
    LexerToken root;

    // Store a map of ParserExpressions to make tokens of the given types.
    std::vector<ParserExpression> tokenMap;
public:
    Parser() = default;

    // Add a new parser expression
    ParserExpression & add_token(std::string token_type){
        tokenMap.emplace_back(token_type);
        return tokenMap.back();
    }

    LexerToken parse_stream(std::vector<LexerToken> stream){
        return LexerToken();
    }
};