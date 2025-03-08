#pragma once

#include "parser/tokens.h"
#include "parser/parse_stack.h"
#include "parser/parser_expression.h"

#include <vector>
#include <optional>
#include <cassert>

// ------------------------- Parser ---------------------- //
//                                                         //
// An object containing a series of rules and expressions  //
// to implement a shift reduce parser for a series of      //
// LexerTokens produced by a Lexer.                        //
//                                                         //
// ------------------------------------------------------- //

class Parser {
    // List of all Parser Expression Rules
    std::vector<ParserExpression> tokenMap;

    // Method to query all rules for the token pattern
    //
    // match_string: The Match String to find in the Expressions
    //
    // return: The Construction Function to make the Expression
    //         matched by the match string. Otherwise nothing.
    std::optional<ConstructionFunction> seek_rule(std::string match_string);
public:
    Parser() = default;

    // Produce a new Parser Expression where all rules produce
    // tokens of type (token_type).
    //
    // token_type: The Type to be produced by all rules.
    //
    // return: Reference to the produced parser expression (to
    //         be chained with add_rule)
    ParserExpression & add_token(std::string token_type);

    // Parse a stream of LexerTokens and return the root of the AST
    //
    // token_stream: The sequence of tokens produced by the Lexer
    //
    // return: The root of the AST
    std::unique_ptr<Token> parse_stream(std::vector<std::unique_ptr<LexerToken>>&& token_stream);
};