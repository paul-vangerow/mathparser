#pragma once

#include "lexer/lexer.h"

class NumericToken : public LexerToken {
public:
    NumericToken(std::string type, std::vector<LexerToken> in) 
    : LexerToken(type)
    {
        assert(in.size() == 1);
        assert(in[0].get_type() == "NUM");
        connections.push_back(in[0]);
    }

    void print(){
        std::cout << "NUM {";
        connections[0].print();
        std::cout << "}";
    }
};

class VariableToken : public LexerToken {
public:
    VariableToken(std::string type, std::vector<LexerToken> in) 
    : LexerToken(type)
    {
        assert(in.size() == 1);
        assert(in[0].get_type() == "VAR");
        connections.push_back(in[0]);
    }

    void print(){
        std::cout << "VAR {";
        connections[0].print();
        std::cout << "}";
    }
};

class AddExprToken : public LexerToken {
public:
    AddExprToken(std::string type, std::vector<LexerToken> in) 
    : LexerToken(type)
    {
        assert(in.size() == 3);
        assert(in[0].get_type() == "EXPR");
        assert(in[1].get_type() == "PLUS");
        assert(in[2].get_type() == "EXPR");
        connections.push_back(in[0]);
        connections.push_back(in[2]);
    }
    void print(){
        connections[0].print();
        std::cout << " + ";
        connections[1].print();
    }
};

class EquationToken : public LexerToken {
public:
    EquationToken(std::string type, std::vector<LexerToken> in) 
    : LexerToken(type)
    {
        assert(in.size() == 3);
        assert(in[0].get_type() == "EXPR");
        assert(in[1].get_type() == "EQ");
        assert(in[2].get_type() == "EXPR");
        connections.push_back(in[0]);
        connections.push_back(in[2]);
    }
    void print(){
        connections[0].print();
        std::cout << " = ";
        connections[1].print();
    }
};

class EquationSet : public LexerToken {
public:
    EquationSet(std::string type, std::vector<LexerToken> in) 
    : LexerToken(type)
    {
        if (in.size() == 1){
            assert(in[0].get_type() == "EQUATION");
            connections.push_back(in[0]);
        } else {
            assert(in.size() == 3);
            assert(in[0].get_type() == "EQUATION_SET");
            assert(in[1].get_type() == "EOL");
            assert(in[2].get_type() == "EQUATION");
            connections.insert(connections.end(), in[0].get_connections().begin(), in[0].get_connections().end());
            connections.push_back(in[2]);
        }
        connections.push_back(in[0]);
        connections.push_back(in[2]);
    }

    void print(){
        for (auto item : connections){
            item.print();
            std::cout << "\n";
        }
    }
};
