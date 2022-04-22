#ifndef ANALYSE_SYNTAXIQUE_H
#define ANALYSE_SYNTAXIQUE_H

#include "analyse_lexicale.h"

#include <unordered_map>

namespace asmb
{
    opcode::opcode_t opcode_depuis_token(const Token &token);

    Token defiler(std::queue<Token> &file);

    opcode::instruction_t cbg(const int &numero_registre, const int &immediat);
    opcode::instruction_t cbd(const int &numero_registre, const int &immediat);

    opcode::id_reg_t rd(const int &numero_registre);
    opcode::id_reg_t rt(const int &numero_registre);
    opcode::id_reg_t rs(const int &numero_registre);

    reg_t *analyse_syntaxique(std::queue<Token> &file_tokens);
}

#endif