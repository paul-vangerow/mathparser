#pragma once

#include <iostream>
#include <vector>
#include <ostream>

// This file contains all the Token definitions to be
// assembled into an AST by the parser.

class Token {
private:
    std::string dynamic_type;
    std::string simplification_type;
public:
    Token() = default;
    Token(Token&& other) : dynamic_type(other.type()), simplification_type(other.simp_type()){};
    Token(std::string type) : dynamic_type(type) {}
    Token(std::string type, std::string simp_type) : dynamic_type(type), simplification_type(simp_type) {}
    
    virtual ~Token() = default;
    void operator=(Token&& other){
        dynamic_type = other.type();
        simplification_type = other.simp_type();
    }

    std::string type(){
        return dynamic_type;
    }
    std::string simp_type(){
        return simplification_type;
    }

    virtual std::vector<Token*> get() { return std::vector<Token*>{}; }
    virtual void print(std::ostream& stream) {
        stream << "base";
    }
};

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

    void print(std::ostream& stream) override {
        stream << content();
    }
};

class NumericToken : public Token {
private:
    std::unique_ptr<Token> value;
public:
    NumericToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "NUMERIC")
    {
        assert(in.size() == 1);
        assert(in[0]->type() == "NUM");
        value = std::move(in[0]);
    }

    void print(std::ostream& stream) override {
        stream << "NUM {";
        value->print(stream);
        stream << "}";
    }

    std::vector<Token*> get() override { 
        return std::vector<Token*>{value.get()}; 
    }
};

class VariableToken : public Token {
private:
    std::unique_ptr<Token> value;
public:
    VariableToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "VARIABLE")
    {
        assert(in.size() == 1);
        assert(in[0]->type() == "VAR");
        value = std::move(in[0]);
    }

    void print(std::ostream& stream) override {
        stream << "VAR {";
        value->print(stream);
        stream << "}";
    }

    std::vector<Token*> get() override { 
        return std::vector<Token*>{value.get()}; 
    }
};

class AddExprToken : public Token {
private:
    std::unique_ptr<Token> expr1;
    std::unique_ptr<Token> expr2;
public:
    AddExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "ADDITION")
    {
        assert(in.size() == 3);
        assert(in[0]->type() == "EXPR");
        assert(in[1]->type() == "ADD");
        assert(in[2]->type() == "EXPR");
        expr1 = std::move(in[0]);
        expr2 = std::move(in[2]);
    }
    void print(std::ostream& stream) override {
        expr1->print(stream);
        stream << " + ";
        expr2->print(stream);
    }

    std::vector<Token*> get() override { 
        return std::vector<Token*>{expr1.get(), expr2.get()}; 
    }
};

class BrExprToken : public Token {
private:
    std::unique_ptr<Token> expr;
public:
    BrExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "BRACKETED")
    {
        assert(in.size() == 3);
        assert(in[0]->type() == "BRO");
        assert(in[1]->type() == "EXPR");
        assert(in[2]->type() == "BRC");
        expr = std::move(in[1]);
    }
    void print(std::ostream& stream) override {
        stream << "( ";
        expr->print(stream);
        stream << " )";
    }

    std::vector<Token*> get() override { 
        return std::vector<Token*>{expr.get()}; 
    }
};

class SubExprToken : public Token {
private:
    std::unique_ptr<Token> expr1;
    std::unique_ptr<Token> expr2;
public:
    SubExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "SUBTRACTION")
    {
        assert(in.size() == 3);
        assert(in[0]->type() == "EXPR");
        assert(in[1]->type() == "SUB");
        assert(in[2]->type() == "EXPR");
        expr1 = std::move(in[0]);
        expr2 = std::move(in[2]);
    }
    void print(std::ostream& stream) override {
        expr1->print(stream);
        stream << " - ";
        expr2->print(stream);
    }

    std::vector<Token*> get() override { 
        return std::vector<Token*>{expr1.get(), expr2.get()}; 
    }
};

class MulExprToken : public Token {
private:
    std::unique_ptr<Token> expr1;
    std::unique_ptr<Token> expr2;
public:
    MulExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "MULTIPLY")
    {
        if (in.size() == 2){
            assert(in[0]->type() == "EXPR");
            assert(in[1]->type() == "EXPR");
            expr1 = std::move(in[0]);
            expr2 = std::move(in[1]);
        } else {
            assert(in.size() == 3);
            assert(in[0]->type() == "EXPR");
            assert(in[1]->type() == "MUL");
            assert(in[2]->type() == "EXPR");
            expr1 = std::move(in[0]);
            expr2 = std::move(in[2]);
        }
    }

    void print(std::ostream& stream) override {
        expr1->print(stream);
        stream << " * ";
        expr2->print(stream);
    }

    std::vector<Token*> get() override { 
        return std::vector<Token*>{expr1.get(), expr2.get()}; 
    }
};

class EquationToken : public Token {
private:
    std::unique_ptr<Token> expr1;
    std::unique_ptr<Token> expr2;
public:
    EquationToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "EQUATION")
    {
        assert(in.size() == 3);
        assert(in[0]->type() == "EXPR");
        assert(in[1]->type() == "EQ");
        assert(in[2]->type() == "EXPR");
        expr1 = std::move(in[0]);
        expr2 = std::move(in[2]);
    }

    void print(std::ostream& stream) override {
        expr1->print(stream);
        stream << " = ";
        expr2->print(stream);
    }

    std::vector<Token*> get() override { 
        return std::vector<Token*>{expr1.get(), expr2.get()}; 
    }
};

class EquationSet : public Token {
private:
    std::unique_ptr<Token> equation_set1;
    std::unique_ptr<Token> equation_set2;
public:
    EquationSet(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type, "EQUATION_LINK")
    {
        if (in.size() == 1){
            assert(in[0]->type() == "EQUATION");
            equation_set1 = std::move(in[0]);
        } else {
            assert(in.size() == 3);
            assert(in[0]->type() == "EQUATION_SET");
            assert(in[1]->type() == "EOL");
            assert(in[2]->type() == "EQUATION_SET");
            equation_set1 = std::move(in[0]);
            equation_set2 = std::move(in[2]);
        }
    }

    void print(std::ostream& stream) override {
        if (equation_set2){
            equation_set1->print(stream);
            stream << "\n";
            equation_set2->print(stream);
        } else {
            equation_set1->print(stream);
        }
    }

    std::vector<Token*> get() override {
        if (equation_set2){
            return std::vector<Token*>{equation_set1.get(), equation_set2.get()}; 
        } else {
            return std::vector<Token*>{equation_set1.get()}; 
        }
    }
};
