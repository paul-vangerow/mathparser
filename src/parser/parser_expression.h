#pragma once

#include "parser/tokens.h"

#include <vector>
#include <optional>

// -------------------- ParserExpression ----------------- //
//                                                         //
// An object containing a set of rules to construct a      //
// specific kind of Parse Token. Contains methods to       //
// construct and search for rules based on match_strings   //
// created by the parse_stack.                             //
//                                                         //
// ------------------------------------------------------- //

// Alias for the function definition of a Token Construction 
// Function 
using ConstructionFunction = std::function<std::unique_ptr<Token>(std::vector<std::unique_ptr<Token>>&&)>;

class ParserExpression {
    // Encapsulated type for a Parser Rule with the method
    // to construct the appropriate type as well as the
    // sequence of types which can result in its construction.
    struct ParseRule {
        std::string match_sequence;
        ConstructionFunction construction_rule;
    };

    // The resultant type of any rule which matches in this
    // expression.
    std::string m_type;

    // The set of all Rules which can make the resultant type.
    std::vector<ParseRule> m_rules;
public:
    ParserExpression(std::string token_type) : m_type(token_type) {}

    // Templated method to add a construction rule to the expression.
    //
    // T: The Type of the produced token (Inherits from Token) and
    //    constructed from a sequence of other tokens.
    // match_sequence: The Type String which results in the invocation
    //    of this construction rule.
    //
    // return: A reference to the expression (to allow for nice 
    //    function call chaining).
    template <typename T>
    ParserExpression & add_rule(std::string match_sequence){
        m_rules.push_back({match_sequence, [type_string = m_type](std::vector<std::unique_ptr<Token>>&& in){return std::make_unique<T>(type_string, std::move(in));}});
        return *this;
    }

    // A method to search all rules in the current expression for the
    // specified match string.
    //
    // match_string: The match_string to search for
    //
    // return: The ConstructionFunction if it is found, otherwise nothing.
    std::optional<ConstructionFunction> check_match(std::string match_string);
};