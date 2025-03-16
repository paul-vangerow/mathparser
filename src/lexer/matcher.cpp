#include "lexer/matcher.h"

Matcher::MatchLayer::MatchLayer(
    Matcher * __parent__, 
    std::vector<LexerSequence> sequences, 
    std::vector<std::unique_ptr<LexerToken>>&& tokens)
: parent(__parent__)
, active_sequences(sequences)
, current_tokens(std::move(tokens)){}

// Return how many sequences are left, if this number drops to 0
// then the layer needs to be removed and replaced.
int Matcher::MatchLayer::match_token(char c){
    bool layer_created = false;

    active_content += c;

    // Match each sequence.
    for (auto seq_it = active_sequences.begin(); seq_it != active_sequences.end();){
        MatchResult res = seq_it->match_token(c);

        // Create a new layer (replace all existing layers) and destroy all subsequent sequences.
        if (res.is_end) {
            layer_created = true;
            next = parent->construct_new_layer(current_tokens, std::make_unique<LexerToken>(seq_it->get_token(), active_content));
            active_sequences.erase(std::next(seq_it), active_sequences.end());
        }
        // Destroy current sequence.
        if (res.is_empty){
            seq_it = active_sequences.erase(seq_it);
        } else {
            ++seq_it;
        }
    }

    // If layers weren't created, continue call recursively.
    if (next && !layer_created){
        int remaining_sequences = next->match_token(c);
        if (remaining_sequences == 0){
            if (next->next){
                auto tmp = std::move(next->next);
                next = std::move(tmp);
            } else {
                next.reset();
            }
        }
    }
    return active_sequences.size();
}

std::unique_ptr<Matcher::MatchLayer> Matcher::construct_new_layer(std::vector<std::unique_ptr<LexerToken>>& current_tokens, std::unique_ptr<LexerToken> matched_token){
    std::vector<std::unique_ptr<LexerToken>> new_tokens;
    for (auto& item : current_tokens){
        new_tokens.emplace_back(new LexerToken(item));
    }
    if (matched_token->get_dtype() != "UNIMPLEMENTED"){
        new_tokens.push_back(std::move(matched_token));
    }
    return std::make_unique<MatchLayer>(this, original, std::move(new_tokens));
}

std::unique_ptr<Matcher::MatchLayer> Matcher::construct_new_layer(){
    std::vector<std::unique_ptr<LexerToken>> empty_tokens;
    std::unique_ptr<LexerToken> fill_token = std::make_unique<LexerToken>();
    
    return construct_new_layer(empty_tokens, std::move(fill_token));
}

// Initialise by creating first layer.
Matcher::Matcher(std::vector<LexerSequence>& __original__) : original(__original__) {
    top = construct_new_layer();
}

// Change signature to have an error type in case nothing can be matched?
void Matcher::match_token(char c){
    int remaining_sequences = top->match_token(c);
    if (remaining_sequences == 0){
        if (top->next){
            auto tmp = std::move(top->next);
            top = std::move(tmp);
        } else {
            // Throw error (this means NOTHING could match)
            top = construct_new_layer();
        }
    }
}

std::vector<std::unique_ptr<LexerToken>> Matcher::get_tokens(){
    if (top->next) return std::move(top->next->current_tokens);
    else return std::move(top->current_tokens);
}