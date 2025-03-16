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

std::size_t Parser::get_max_depth(Token* root){
    std::size_t max_depth = 0;
    for (auto& item : root->get()){
        max_depth = std::max(max_depth, get_max_depth(item));
    }
    return 1 + max_depth;
}

std::size_t Parser::navigate_layers(Token* root, std::size_t depth, std::size_t min_width, std::vector<std::string>& layer_strings){
    std::string append = " " + root->simp_type() + " ";

    if (append.size() < min_width) {
        std::size_t initial_pad = min_width - append.size();
        std::size_t l_pad = initial_pad / 2;
        std::size_t r_pad = initial_pad - l_pad;

        append = ( std::string(l_pad, ' ') + append + std::string(r_pad, ' ') );
    }

    auto connections = root->get();
    if (connections.size() == 0){
        layer_strings[depth] += append;

        std::string fill(append.size(), ' ');
        for (std::size_t further = depth + 1; further < layer_strings.size(); further++){
            layer_strings[further] += fill;
        }
        return append.size();
    } else {
        std::size_t width = 0;
        for (auto& item : connections){
            width += navigate_layers(item, depth + 1, append.size() / connections.size(), layer_strings );
        }
        std::size_t total_pad = width - append.size();

        std::size_t l_pad = total_pad / 2;
        std::size_t r_pad = total_pad - l_pad;

        append = ( std::string(l_pad, ' ') + append + std::string(r_pad, ' ') );

        layer_strings[depth] += append;

        return append.size();
    }
}

void Parser::print_as_tree(Token* root){
    std::vector<std::string> layer_strings(get_max_depth(root));
    navigate_layers(root, 0, 0, layer_strings);

    for (auto layer : layer_strings){
        std::cout << layer << "\n";
    }
}