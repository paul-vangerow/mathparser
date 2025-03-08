#include "parser/parser.h"

std::optional<ConstructionFunction> Parser::seek_rule(std::string match_string){
    for (auto& item : tokenMap){
        std::optional<ConstructionFunction> value = item.check_match(match_string);
        if (value) return value;
    }
    return {};
}

ParserExpression & Parser::add_token(std::string token_type){
    tokenMap.emplace_back(token_type);
    return tokenMap.back();
}

std::unique_ptr<Token> Parser::parse_stream(std::vector<std::unique_ptr<LexerToken>>&& token_stream){
    ParseStack stack;
    for (auto it = token_stream.begin(); it != token_stream.end(); ++it){
        std::string type = (*it)->type();
        stack.push(std::move(*it));

        // Peek type of next token
        std::string peek_next = "";
        if ( std::next(it) != token_stream.end() ){
            peek_next = (*std::next(it))->type();
        }
        
        bool reduce = true;

        // If the stack got reduced, run again.
        while(reduce){
            reduce = false;
            for (std::size_t elements = 1; elements <= stack.size(); elements++){
                std::string match_string = stack.peek_n(elements);

                auto check_rule = seek_rule(match_string);

                // If there is no rule to make the current token sequence, keep looking.
                if (!check_rule) continue;

                std::string confirm_string = match_string + " " + peek_next;
                auto peek_rule = seek_rule(confirm_string);
                
                // We have a rule for the current sequence AND there is a rule with the
                // next token. We are GREEDY so we keep going (get most tokens possible).
                if (peek_rule) break;

                // This is the last element with which we have a sequence. Reduce the stack.
                std::vector<std::unique_ptr<Token>> to_collapse = stack.pop_n(elements);

                // Build new token value
                std::unique_ptr<Token> new_element = (*check_rule)(std::move(to_collapse));

                stack.push(std::move(new_element));
                reduce = true;
                break;
            }
        }
    }
    return stack.get();
}