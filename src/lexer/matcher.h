#pragma once

#include <vector>
#include <lexer/lexer_sequence.h>
#include <parser/tokens.h>

class LexerToken : public Token{
private:
    std::string m_content;
public:
    LexerToken(LexerToken&& other) : Token(other.type()), m_content(other.content()) {}

    LexerToken() 
    : Token("UNIMPLEMENTED")
    , m_content("N/A"){}

    LexerToken(std::string type, std::string content)
    : Token(type)
    , m_content(content){}

    LexerToken(std::string type)
    : Token(type)
    , m_content("N/A") {}

    LexerToken(std::unique_ptr<LexerToken>& v)
    : Token(v->type())
    , m_content(v->content()) {}

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
        std::vector<std::unique_ptr<LexerToken>> current_tokens;
        std::string active_content;
        std::unique_ptr<MatchLayer> next;

        MatchLayer(Matcher * __parent__, std::vector<LexerSequence> sequences, std::vector<std::unique_ptr<LexerToken>>&& tokens);

        int match_token(char c);
    };

    std::vector<LexerSequence>& original;
    std::unique_ptr<MatchLayer> top;
public:
    std::unique_ptr<MatchLayer> construct_new_layer(std::vector<std::unique_ptr<LexerToken>>& current_tokens, std::unique_ptr<LexerToken> matched_token);
    std::unique_ptr<MatchLayer> construct_new_layer();

    Matcher(std::vector<LexerSequence>& __original__);

    void match_token(char c);

    std::vector<std::unique_ptr<LexerToken>> get_tokens();
};