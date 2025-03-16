#pragma once

#include "src/parser/tokens.h"

namespace simplification_rules {

static void remove_brackets(std::unique_ptr<Token>& ast_root){
    // The root will never be bracketed so check children only.

    // auto children = ast_root->get();
    // for (auto& child : children){
    //     if (child->simp_type() == "BRACKETED"){

    //     }
    // }
}

static void distribute_multiply(std::unique_ptr<Token>& ast_root){

}

}