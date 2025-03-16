#pragma once

#include <iostream>
#include <vector>
#include <ostream>

// This file contains all the Token definitions to be
// assembled into an AST by the parser.

class Token {
private:
    std::string m_dtype;
    std::string m_stype;
protected:
    std::vector<std::unique_ptr<Token>> m_children;
public:
    // Default constructor
    Token() = default;
    
    Token(std::string dtype)
    : m_dtype(dtype) 
    {}

    Token(std::string dtype, std::string stype) 
    : m_dtype(dtype)
    , m_stype(stype) 
    {}
    
    virtual ~Token() = default;
    void operator=(Token&& other){
        m_dtype = other.get_dtype();
        m_stype = other.get_stype();
    }

    std::string get_dtype(){
        return m_dtype;
    }
    std::string get_stype(){
        return m_stype;
    }

    virtual std::vector<std::unique_ptr<Token>>& get_children(){ 
        return m_children; 
    }

    virtual void print(std::ostream& stream) {
        stream << "base";
    }
};

class LexerToken : public Token{
private:
    std::string m_content;
public:
    // LexerToken(LexerToken&& other) : Token(other.type()), m_content(other.content()) {}

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
    : Token(v->get_dtype())
    , m_content(v->content()) {}

    std::string content(){
        return m_content;
    }

    void print(std::ostream& stream) override {
        stream << content();
    }
};
