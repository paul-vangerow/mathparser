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

class NumericToken : public Token {
public:
    NumericToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "NUMERIC")
    {
        assert(in.size() == 1);
        assert(in[0]->get_dtype() == "NUM");
        m_children.push_back(std::move(in[0]));
    }

    void print(std::ostream& stream) override {
        stream << "NUM {";
        m_children[0]->print(stream);
        stream << "}";
    }
};

class VariableToken : public Token {
public:
    VariableToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "VARIABLE")
    {
        assert(in.size() == 1);
        assert(in[0]->get_dtype() == "VAR");
        m_children.push_back(std::move(in[0]));
    }

    void print(std::ostream& stream) override {
        stream << "VAR {";
        m_children[0]->print(stream);
        stream << "}";
    }
};

class AddExprToken : public Token {
public:
    AddExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "ADDITION")
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

class BrExprToken : public Token {
public:
    BrExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "BRACKETED")
    {
        assert(in.size() == 3);
        assert(in[0]->get_dtype() == "BRO");
        assert(in[1]->get_dtype() == "EXPR");
        assert(in[2]->get_dtype() == "BRC");
        m_children.push_back(std::move(in[1]));
    }
    void print(std::ostream& stream) override {
        stream << "( ";
        m_children[0]->print(stream);
        stream << " )";
    }
};

class SubExprToken : public Token {
public:
    SubExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "SUBTRACTION")
    {
        assert(in.size() == 3);
        assert(in[0]->get_dtype() == "EXPR");
        assert(in[1]->get_dtype() == "SUB");
        assert(in[2]->get_dtype() == "EXPR");
        m_children.push_back(std::move(in[0]));
        m_children.push_back(std::move(in[2]));
    }
    void print(std::ostream& stream) override {
        m_children[0]->print(stream);
        stream << " - ";
        m_children[1]->print(stream);
    }
};

class MulExprToken : public Token {
public:
    MulExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "MULTIPLY")
    {
        if (in.size() == 2){
            assert(in[0]->get_dtype() == "EXPR");
            assert(in[1]->get_dtype() == "EXPR");
            m_children.push_back(std::move(in[0]));
            m_children.push_back(std::move(in[1]));
        } else {
            assert(in.size() == 3);
            assert(in[0]->get_dtype() == "EXPR");
            assert(in[1]->get_dtype() == "MUL");
            assert(in[2]->get_dtype() == "EXPR");
            m_children.push_back(std::move(in[0]));
            m_children.push_back(std::move(in[2]));
        }
    }

    void print(std::ostream& stream) override {
        m_children[0]->print(stream);
        stream << " * ";
        m_children[1]->print(stream);
    }
};

class EquationToken : public Token {
public:
    EquationToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
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

class EquationSet : public Token {
public:
    EquationSet(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "EQUATION_LINK")
    {
        if (in.size() == 1){
            assert(in[0]->get_dtype() == "EQUATION");
            m_children.push_back(std::move(in[0]));
        } else {
            assert(in.size() == 3);
            assert(in[0]->get_dtype() == "EQUATION_SET");
            assert(in[1]->get_dtype() == "EOL");
            assert(in[2]->get_dtype() == "EQUATION_SET");

            for (auto&& item : in[0]->get_children()){
                m_children.push_back(std::move(item));
            }
            for (auto&& item : in[2]->get_children()){
                m_children.push_back(std::move(item));
            }
        }
    }

    void print(std::ostream& stream) override {
        for (auto& equation : m_children){
            equation->print(stream);
            stream << "\n";
        }
    }
};
