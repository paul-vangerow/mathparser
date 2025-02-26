#pragma once

#include <iostream>
#include <vector>

class Token {
private:
    std::string dynamic_type;
public:
    Token() = default;
    Token(std::string type) : dynamic_type(type) {}
    virtual ~Token() = default;

    std::string type(){
        return dynamic_type;
    }

    virtual void print() {}
};

// All value components are stored in the BASE CLASS. Derived
// class components here exist purely for the sake of readability.

class NumericToken : public Token {
private:
    std::unique_ptr<Token> value;
public:
    NumericToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type)
    {
        assert(in.size() == 1);
        assert(in[0]->type() == "NUM");
        value = std::move(in[0]);
    }

    void print(){
        std::cout << "NUM {";
        value->print();
        std::cout << "}";
    }
};

class VariableToken : public Token {
private:
    std::unique_ptr<Token> value;
public:
    VariableToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type)
    {
        assert(in.size() == 1);
        assert(in[0]->type() == "VAR");
        value = std::move(in[0]);
    }

    void print(){
        std::cout << "VAR {";
        value->print();
        std::cout << "}";
    }
};

class AddExprToken : public Token {
private:
    std::unique_ptr<Token> expr1;
    std::unique_ptr<Token> expr2;
public:
    AddExprToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type)
    {
        assert(in.size() == 3);
        assert(in[0]->type() == "EXPR");
        assert(in[1]->type() == "PLUS");
        assert(in[2]->type() == "EXPR");
        expr1 = std::move(in[0]);
        expr2 = std::move(in[2]);
    }
    void print(){
        expr1->print();
        std::cout << " + ";
        expr2->print();
    }
};

class EquationToken : public Token {
private:
    std::unique_ptr<Token> expr1;
    std::unique_ptr<Token> expr2;
public:
    EquationToken(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type)
    {
        assert(in.size() == 3);
        assert(in[0]->type() == "EXPR");
        assert(in[1]->type() == "EQ");
        assert(in[2]->type() == "EXPR");
        expr1 = std::move(in[0]);
        expr2 = std::move(in[2]);
    }
    void print(){
        expr1->print();
        std::cout << " = ";
        expr2->print();
    }
};

class EquationSet : public Token {
private:
    std::unique_ptr<Token> equation_set;
    std::unique_ptr<Token> equation;
public:
    EquationSet(std::string type, std::vector<std::unique_ptr<Token>> in) 
    : Token(type)
    {
        if (in.size() == 1){
            assert(in[0]->type() == "EQUATION");
            equation = std::move(in[0]);
        } else {
            assert(in.size() == 3);
            assert(in[0]->type() == "EQUATION_SET");
            assert(in[1]->type() == "EOL");
            assert(in[2]->type() == "EQUATION");
            equation_set = std::move(in[0]);
            equation = std::move(in[2]);
        }
    }

    void print(){
        if (equation_set){
            equation_set->print();
            std::cout << "\n";
        }
        equation->print();
    }
};
