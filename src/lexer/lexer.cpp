#include "lexer/lexer.h"


// To be moved later

class Matcher {
    struct MatchLayer {
        Matcher * parent;
        std::vector<LexerSequence> active_sequences;
        std::vector<std::string> current_tokens;
        std::unique_ptr<MatchLayer> next;

        MatchLayer(
            Matcher * __parent__,
            std::vector<LexerSequence> sequences,
            std::vector<std::string>&& tokens)
        : parent(__parent__)
        , active_sequences(sequences)
        , current_tokens(tokens){}

        // Return how many sequences are left, if this number drops to 0
        // then the layer needs to be removed and replaced.
        int match_token(char c){
            bool layer_created = false;

            // Match each sequence.
            for (auto seq_it = active_sequences.begin(); seq_it != active_sequences.end();){
                MatchResult res = seq_it->match_token(c);

                // Create a new layer (replace all existing layers) and destroy all subsequent sequences.
                if (res.is_end) {
                    layer_created = true;
                    next = parent->construct_new_layer(current_tokens, seq_it->get_token());
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
    };

    std::vector<LexerSequence>& original;
    std::unique_ptr<MatchLayer> top;

    std::vector<std::string> out_tokens;
public:
    std::unique_ptr<MatchLayer> construct_new_layer(std::vector<std::string> current_tokens, std::string matched_token = ""){
        if (matched_token != ""){
            current_tokens.push_back(matched_token);
        }
       return std::make_unique<MatchLayer>(this, original, std::move(current_tokens));
    }

    // Initialise by creating first layer.
    Matcher(std::vector<LexerSequence>& __original__) : original(__original__) {
        top = construct_new_layer(std::vector<std::string>{});
    }

    // Change signature to have an error type in case nothing can be matched?
    void match_token(char c){
        int remaining_sequences = top->match_token(c);
        if (remaining_sequences == 0){
            if (top->next){
                auto tmp = std::move(top->next);
                top = std::move(tmp);
            } else {
                // Throw error (this means NOTHING could match)
                top = construct_new_layer(std::vector<std::string>{});
            }
        }
    }

    std::vector<std::string> get_tokens(){
        if (top->next) return top->next->current_tokens;
        else return top->current_tokens;
    }
};

/// -------

void Lexer::addSequence(std::string token, std::string match){
    m_sequences.emplace_back(token, match);
}

std::vector<std::string> Lexer::match_sequence(std::string token_sequence){
    Matcher matcher(m_sequences);
    for (auto c : token_sequence){
        matcher.match_token(c);
    }
    return matcher.get_tokens();
}