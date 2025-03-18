
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

    std::string input_sequence = arg_parse.get_input_stream(arg_parse["input_path"]);

    if (input_sequence.size() == 0) return 1;

    std::unique_ptr<std::ofstream> output_stream;;
    if (arg_parse["output_path"].size()){
        output_stream = arg_parse.get_output_stream(arg_parse["output_path"]);
    }

    Lexer math_lexer;

    // Define Lexer Tokens //
    math_lexer.addSequence("NUM", "[0-9]+(.[0-9]+)?");
    math_lexer.addSequence("VAR", "[a-zA-Z]+");
    math_lexer.addSequence("EQ", "=");
    math_lexer.addSequence("BRO", "/(");
    math_lexer.addSequence("BRC", "/)");
    math_lexer.addSequence("DIV", "//");
    math_lexer.addSequence("MUL", "/*");
    math_lexer.addSequence("SUB", "-");
    math_lexer.addSequence("ADD", "/+");
    math_lexer.addSequence("EOL", "(\n)+");
    math_lexer.addSequence("UNIMPLEMENTED", " +");

    // Actually do the parsing and lexing //

    std::vector<std::unique_ptr<LexerToken>> out = math_lexer.match_sequence(input_sequence);
    math_lexer.print_sequence(out);

    return 0;
}