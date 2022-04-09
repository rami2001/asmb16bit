#ifndef ASM_H
#define ASM_H

#include "opcodes.h"

#include <string>
#include <algorithm>

namespace asmb
{
    constexpr codage::instruction_t ERR { 0 };

    std::string chaineFormelle(const std::string &instruction);

    codage::opcode_t opcodeDepuisChaine(const std::string &instruction);

    int numRegistreDepuisCaractere(const char caractere);

    codage::opcode::idReg_t rdDepuisChaine(const std::string &instruction);
    codage::opcode::idReg_t rtDepuisChaine(const std::string &instruction);
    codage::opcode::idReg_t rsDepuisChaine(const std::string &instruction);
    
    codage::instruction_t  instructionDepuisChaine(const std::string &instruction);

    bool instructionEstValide();
}

#endif