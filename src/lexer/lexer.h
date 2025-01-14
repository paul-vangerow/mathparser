#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <memory>

#include "lexer/lexer_sequence.h"

// This is just a wrapper to make it convenient to copy the original sequences objects
class PrimarySequences {
private:
    std::vector<std::unique_ptr<LexerSequence>> sequences;
public:
    PrimarySequences() = default;

    void addSequence(std::string token, std::string match){
        std::cout << "Sequence Created \n";
        sequences.emplace_back( new LexerSequence(token, match) );
        std::cout << "Sequence added \n";
    }

    std::vector<std::unique_ptr<LexerSequence>> make_copy(){
        std::vector<std::unique_ptr<LexerSequence>> cpy;
        for (auto & item : sequences){
            cpy.push_back( std::make_unique<LexerSequence>(*item) );
        }
        return cpy;
    }
};

struct MatchLayer {
    // Reference LexerSequence to build new layers.
    std::shared_ptr<PrimarySequences> original;

    // Layer Sequences
    std::vector<std::unique_ptr<LexerSequence>> layer_seq;
    std::vector<std::string> tokens;

    // Ref to next layer (allows cascade deletion)
    std::unique_ptr<MatchLayer> next;

    MatchLayer(std::vector<std::string> starter_tokens, std::shared_ptr<PrimarySequences>& __original__){
        original = __original__;
        tokens = std::move(starter_tokens);

        layer_seq = original->make_copy();
    }

    void match(char c){
        for ( auto it = layer_seq.begin(); it != layer_seq.end();){
            auto & sequence = *it;
            std::cout << "Matching sequence with token " << sequence->get_token() << " ";
            MatchResult result = sequence->match(c);
            
            // We have not matched anything and we aren't able to match any more
            if ( !result.is_end && result.is_empty ){
                it = layer_seq.erase(it);
                continue;
            }
            if ( result.is_end ){
                build_layer(sequence);

                // We have matched something, and cannot match more.
                if ( result.is_empty ){
                    layer_seq.erase(it, layer_seq.end());
                } else if (++it != layer_seq.end()) layer_seq.erase(it, layer_seq.end());
                break;
            }
            ++it;
        }
        // Match other layers.
        if (next) {
            next->match(c);
            if (next->layer_seq.size() == 0){
                if (next->next) {
                    auto && lower_layer = std::move(next->next);
                    next.reset();
                    next = std::move(lower_layer);
                } else next.reset();
            }
        }
    }

    void build_layer(std::unique_ptr<LexerSequence>& sequence){
        next.reset();
        std::vector<std::string> new_tokens;
        std::copy(tokens.begin(), tokens.end(), new_tokens.begin());
        new_tokens.push_back(sequence->get_token());
        next = std::make_unique<MatchLayer>(new_tokens, original);
    }
};

struct MatchLayerTopWrapper {
    std::unique_ptr<MatchLayer> top;
    std::shared_ptr<PrimarySequences> original;

    MatchLayerTopWrapper(std::shared_ptr<PrimarySequences>& __original__){
        original = __original__;
        top = std::make_unique<MatchLayer>(std::vector<std::string>{}, __original__);
    }

    // There should always be a top layer.
    std::vector<std::string> getTokens(){
        return top->tokens;
    }

    int match(char c){
        std::cout << "Call Match! ";
        top->match(c);
        std::cout << "Done Match! ";
        if (top->layer_seq.size() == 0){
            if (top->next) {
                auto && lower_layer = std::move(top->next);
                top.reset();
                top = std::move(lower_layer);
            } else {
                return 1;
            }
        }
        return 0;
    }
};

// ---*--- LEXER ---*--- //

class Lexer {
private:
    std::shared_ptr<PrimarySequences> m_sequences;
public:
    Lexer() = default;

    void addSequence(std::string token, std::string match);

    std::vector<std::string> match_sequence(std::string token_sequence);
};

