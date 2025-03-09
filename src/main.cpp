
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "arg_parser/arg_parser.h"

#include <vector>
#include <iostream>

int main(int argc, char* argv[]){
    // Get arguments
    ArgumentParser arg_parse;

    arg_parse.add_argument("input_path");
    arg_parse.add_argument("output_path", "The file to output simplified expressions to.", "-o", "--output");

    if (!arg_parse.parse_arguments(argc, argv)){
        return 1;
    }

    std::cout << arg_parse["input_path"] << " " << arg_parse["output_path"] << "\n";

    Lexer math_lexer;

    // Define Lexer Tokens //
    math_lexer.addSequence("NUM", "[0-9]+(.[0-9]+)?");
    math_lexer.addSequence("VAR", "[a-zA-Z]+");
    math_lexer.addSequence("EQ", "=");
    math_lexer.addSequence("DIV", "//");
    math_lexer.addSequence("MUL", "/*");
    math_lexer.addSequence("SUB", "-");
    math_lexer.addSequence("ADD", "/+");
    math_lexer.addSequence("EOL", "(\n)+");
    math_lexer.addSequence("UNIMPLEMENTED", " +");

    Parser math_parser;

    // Define Parser Grammar //
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

    // Actually do the parsing and lexing //

    std::vector<std::unique_ptr<LexerToken>> out = math_lexer.match_sequence("2a + b + c = 21 \n a + b + c = 14 \n a + 4b - 2c = 23");

    for (auto& item : out){
        std::cout << item->type() << " ";
    }
    std::cout << "\n";

    std::unique_ptr<Token> ast_root = math_parser.parse_stream(std::move(out));
    math_parser.print_as_tree(ast_root.get());
    ast_root->print(std::cout);

    return 0;
}