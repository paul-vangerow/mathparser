#include <iostream>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include <vector>

int main(int argc, char* argv[]){
    Lexer math_lexer;

    math_lexer.addSequence("NUM", "[0-9]+(.[0-9]+)?");
    math_lexer.addSequence("VAR", "[a-zA-Z]+");
    math_lexer.addSequence("EQ", "=");
    math_lexer.addSequence("DIV", "//");
    math_lexer.addSequence("MUL", "/*");
    math_lexer.addSequence("SUB", "-");
    math_lexer.addSequence("ADD", "/+");
    math_lexer.addSequence("EOL", "(\n)+");
    math_lexer.addSequence("UNIMPLEMENTED", " +");

    std::vector<std::unique_ptr<LexerToken>> out = math_lexer.match_sequence("2a + b + c = 21 \n a + b + c = 14 \n a + 4b - 2c = 23");

    for (auto& item : out){
        std::cout << item->type() << " ";
    }
    std::cout << "\n";

    Parser math_parser;

    math_parser.add_token("EQUATION_SET")
        .add_rule<EquationSet>("EQUATION_SET EOL EQUATION_SET")
        .add_rule<EquationSet>("EQUATION");

    math_parser.add_token("EQUATION")
        .add_rule<EquationToken>("EXPR EQ EXPR");

    math_parser.add_token("EXPR")
        .add_rule<NumericToken>("NUM")
        .add_rule<VariableToken>("VAR")
        .add_rule<MulExprToken>("EXPR EXPR")
        .add_rule<MulExprToken>("EXPR MUL EXPR")
        .add_rule<AddExprToken>("EXPR ADD EXPR")
        .add_rule<SubExprToken>("EXPR SUB EXPR");

    std::unique_ptr<Token> ast_root = math_parser.parse_stream(std::move(out));
    ast_root->print();

    return 0;
}