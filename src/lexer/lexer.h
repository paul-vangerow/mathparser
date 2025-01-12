#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "lexer/lexer_sequence.h"

// This is just a wrapper to make it convenient to copy the original sequences objects
class PrimarySequences {
private:
    std::vector<std::unique_ptr<LexerSequence>> sequences;
public:
    PrimarySequences() = default;

    void addSequence(std::string token, std::string match){
        sequences.push_back( std::make_unique<LexerSequence>(token, match) );
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
};

struct MatchLayerTopWrapper {
    std::unique_ptr<MatchLayer> top;

    MatchLayerTopWrapper(std::shared_ptr<PrimarySequences>& __original__){
        top = std::make_unique<MatchLayer>(std::vector<std::string>{}, __original__);
    }

    // There should always be a top layer.
    std::vector<std::string> getTokens(){
        return top->tokens;
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

