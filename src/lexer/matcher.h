#pragma once

#include "lexer/lexer_sequence.h"
#include "parser/tokens.h"

#include <vector>

// ----------------------- Matcher ----------------------- //
//                                                         //
// A helper object for the process of matching a series of //
// input tokens. Utilises 'Match Layers' to keep track of  //
// which state machines are still relevant after each      //
// input token and what the possible output token lists    //
// are at each stage.                                      //
//                                                         //
// ------------------------------------------------------- //

class Matcher {
    // An object used to keep track of the relevant state
    // machines and which possible tokens have been produced.
    struct MatchLayer {
        // A reference to the matcher (needed for the sake of
        // contstructing new match laters).
        Matcher * parent;

        // A list of all currently active sequences in this
        // match layer (still matching the regex and have
        // higher precedence than already matched sequences)
        std::vector<LexerSequence> active_sequences;

        // A list of all already produced tokens for this 
        // match layer.
        std::vector<std::unique_ptr<Token>> current_tokens;

        // The content for the currently being constructed 
        // Lexer Token.
        std::string active_content;

        // A reference to the next in-line MatchLayer (to 
        // replace this if no sequences match anything)
        std::unique_ptr<MatchLayer> next;

        MatchLayer(Matcher * __parent__, std::vector<LexerSequence> sequences, std::vector<std::unique_ptr<Token>>&& tokens);

        // Progress all active state machines by the next
        // input token. Create or destroy match layers based
        // on the results of the sequences.
        //
        // c: The next input token to be matched
        //
        // return: How many sequences are still matching
        int match_token(char c);
    };

    // A reference to the original set of sequences for the sake
    // of being duplicated (when a new MatchLayer is constructed)
    std::vector<LexerSequence>& original;

    // A reference to the topmost matchlater
    std::unique_ptr<MatchLayer> top;
public:
    // Construct a new MatchLayer when a sequence has successfully
    // matched (add the matched token to the duplicated current
    // tokens list).
    //
    // current_tokens: The list of tokens present in the current layer.
    // matched_tokens: The newly produced token for the current match.
    //
    // return: A unique ptr to a new match layer (to be attached to
    //         whichever layer made a call to this method)
    std::unique_ptr<MatchLayer> construct_new_layer(std::vector<std::unique_ptr<Token>>& current_tokens, std::unique_ptr<Token> matched_token);

    // Construct a new MatchLayer but with default parameters (no
    // current tokens and no matched_token). This is needed for
    // initialisation.
    //
    // return: A unique ptr to a new match layer
    std::unique_ptr<MatchLayer> construct_new_layer();

    Matcher(std::vector<LexerSequence>& __original__);

    // Propagate the token to match to the match layers and ensure
    // that if the top layer fails to match that it is replaced
    // by the preceeding one.
    //
    // c: The next token to match.
    void match_token(char c);

    // After all tokens have been matched to extract the full set
    // of matched tokens from the topmost layer.
    //
    // return: The full list of matched Tokens
    std::vector<std::unique_ptr<Token>> get_tokens();
};