#include "parser/parser_expression.h"

std::optional<ConstructionFunction> ParserExpression::check_match(std::string match_string){
    for (auto& rule : m_rules){
        if (rule.match_sequence == match_string){
            return rule.construction_rule;
        }
    }
    return {};
}