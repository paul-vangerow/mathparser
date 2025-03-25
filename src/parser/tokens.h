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

class EquationLinkToken : public Token {
public:
    EquationLinkToken(
        std::string type, 
        std::vector<std::unique_ptr<Token>>&& in)
    : Token(type, "EQUATION_LINK")
    {
        if (in.size() == 1){
            // Direct conversion
            assert(in[0]->get_dtype() == "EQUATION");
            m_children = std::move(in);
        } else if (in.size() == 3){
            // Merge two equation links together.
            assert(in[0]->get_dtype() == "EQUATION_LINK");
            assert(in[1]->get_dtype() == "EOL");
            assert(in[2]->get_dtype() == "EQUATION_LINK");

            for (auto && equation : in[0]->get_children()){
                m_children.push_back(std::move(equation));
            }
            for (auto && equation : in[2]->get_children()){
                m_children.push_back(std::move(equation));
            }
        } else {
            // Invalid input.
            assert(false);
        }
    }

    void print(std::ostream& stream) override {
        for (auto & item : m_children){
            item->print(stream);
            stream << "\n";
        }
    }
};

class EquationToken : public Token {
public:
    EquationToken(
        std::string type, 
        std::vector<std::unique_ptr<Token>>&& in)
    : Token(type, "EQUATION")
    {
        assert(in.size() == 3);
        assert(in[0]->get_dtype() == "EXPR");
        assert(in[1]->get_dtype() == "EQ");
        assert(in[2]->get_dtype() == "EXPR");

        m_children.push_back(std::move(in[0]));
        m_children.push_back(std::move(in[2]));
    }

    void print(std::ostream& stream) override {
        m_children[0]->print(stream);
        stream << " = ";
        m_children[1]->print(stream);
    }
};

class AddToken : public Token {
public:
    AddToken(
        std::string type, 
        std::vector<std::unique_ptr<Token>>&& in)
    : Token(type, "ADD")
    {
        assert(in.size() == 3);
        assert(in[0]->get_dtype() == "EXPR");
        assert(in[1]->get_dtype() == "ADD");
        assert(in[2]->get_dtype() == "EXPR");

        m_children.push_back(std::move(in[0]));
        m_children.push_back(std::move(in[2]));
    }

    void print(std::ostream& stream) override {
        m_children[0]->print(stream);
        stream << " + ";
        m_children[1]->print(stream);
    }
};

class MulToken : public Token {
public:
    MulToken(
        std::string type, 
        std::vector<std::unique_ptr<Token>>&& in)
    : Token(type, "MUL")
    {
        if (in.size() == 2){
            // Implicit multiply
            assert(in[0]->get_dtype() == "EXPR");
            assert(in[1]->get_dtype() == "EXPR");

            m_children.push_back(std::move(in[0]));
            m_children.push_back(std::move(in[1]));
        } else if (in.size() == 3){
            // Explicit multiply
            assert(in[0]->get_dtype() == "EXPR");
            assert(in[1]->get_dtype() == "MUL");
            assert(in[2]->get_dtype() == "EXPR");

            m_children.push_back(std::move(in[0]));
            m_children.push_back(std::move(in[2]));
        } else {
            // Invalid input
            assert(false);
        }
    }

    void print(std::ostream& stream) override {
        m_children[0]->print(stream);
        stream << " * ";
        m_children[1]->print(stream);
    }
};

class NumToken : public Token {
public:
    NumToken(
        std::string type, 
        std::vector<std::unique_ptr<Token>>&& in)
    : Token(type, "NUM")
    {
        assert(in.size() == 1);
        assert(in[0]->get_dtype() == "NUM");

        m_children.push_back(std::move(in[0]));
    }

    void print(std::ostream& stream) override {
        m_children[0]->print(stream);
    }
};

class VarToken : public Token {
public:
    VarToken(
        std::string type, 
        std::vector<std::unique_ptr<Token>>&& in)
    : Token(type, "VAR")
    {
        assert(in.size() == 1);
        assert(in[0]->get_dtype() == "VAR");

        m_children.push_back(std::move(in[0]));
    }

    void print(std::ostream& stream) override {
        m_children[0]->print(stream);
    }
};
