#pragma once

#include <vector>
#include <lexer/lexer_sequence.h>
#include <parser/tokens.h>

class LexerToken : public Token{
private:
    std::string m_content;
public:
    LexerToken() 
    : Token("UNIMPLEMENTED")
    , m_content("N/A"){}

    LexerToken(std::string type, std::string content)
    : Token(type)
    , m_content(content){}

    LexerToken(std::string type)
    : Token(type)
    , m_content("N/A") {}

    std::string content(){
        return m_content;
    }

    void print() override {
        std::cout << content();
    }
};

class Matcher {
    struct MatchLayer {
        Matcher * parent;
        std::vector<LexerSequence> active_sequences;
        std::vector<LexerToken> current_tokens;
        std::string active_content;
        std::unique_ptr<MatchLayer> next;

        MatchLayer(Matcher * __parent__, std::vector<LexerSequence> sequences, std::vector<LexerToken>&& tokens);

        int match_token(char c);
    };

    std::vector<LexerSequence>& original;
    std::unique_ptr<MatchLayer> top;
public:
    std::unique_ptr<MatchLayer> construct_new_layer(std::vector<LexerToken> current_tokens, LexerToken matched_token = LexerToken());

    Matcher(std::vector<LexerSequence>& __original__);

    void match_token(char c);

    std::vector<LexerToken> get_tokens();
};