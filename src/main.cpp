#include <iostream>
#include "lexer/lexer.h"
#include "parser/parser.h"

int main(int argc, char* argv[]){
    Lexer math_lexer;

    math_lexer.addSequence("NUM", "[0-9]+(.[0-9]+)?");
    math_lexer.addSequence("VAR", "[a-zA-Z]+");
    math_lexer.addSequence("EQ", "=");
    math_lexer.addSequence("DIV", "//");
    math_lexer.addSequence("MUL", "/*");
    math_lexer.addSequence("MINS", "-");
    math_lexer.addSequence("PLUS", "/+");
    math_lexer.addSequence("EOL", "(\n)+");
    math_lexer.addSequence("", " +"); // Discard whitespace

    std::vector<LexerToken> out = math_lexer.match_sequence("a+20=75 \n b+40=20");

    for (auto item : out){
        std::cout << item.get_type() << " ";
    }

    Parser math_parser;

    math_parser.add_token("EQUATION_SET")
        .add_rule<EquationSet>("EQUATION_SET EOL EQUATION")
        .add_rule<EquationSet>("EQUATION");

    math_parser.add_token("EQUATION")
        .add_rule<EquationToken>("EXPR EQ EXPR");

    math_parser.add_token("EXPR")
        .add_rule<NumericToken>("NUM")
        .add_rule<VariableToken>("VAR")
        .add_rule<AddExprToken>("EXPR ADD EXPR");

    LexerToken ast_root = math_parser.parse_stream(out);
    ast_root.print();

    return 0;
}