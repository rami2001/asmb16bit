#ifndef ASM_H
#define ASM_H

#include "opcodes.h"

#include <string>
#include <algorithm>

namespace asmb
{
    constexpr codage::instruction_t ERR { 0 };

    std::string chaine_formelle(const std::string &instruction);

    codage::opcode_t opcode_depuis_chaine(const std::string &instruction);

    int num_registre_depuis_caractere(const char &caractere);

    codage::opcode::id_reg_t rd_depuis_chaine(const std::string &instruction);
    codage::opcode::id_reg_t rt_depuis_chaine(const std::string &instruction);
    codage::opcode::id_reg_t rs_depuis_chaine(const std::string &instruction);
    
    codage::instruction_t  instruction_depuis_chaine(const std::string &instruction);
}

#endif