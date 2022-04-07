#ifndef ASM_H
#define ASM_H

#include "opcodes.h"

#include <cstring>

namespace asmb
{
    static codage::instruction_t g_instruction { 0 };

    void erreur(void);

    char *chaineFormelle(const char* instruction);

    codage::opcode_t opcodeDepuisChaine(const char *instruction);

    int numRegistreDepuisCaractere(const char caractere);

    codage::opcode::idReg_t rdDepuisChaine(const char *instruction);
    codage::opcode::idReg_t rtDepuisChaine(const char *instruction);
    codage::opcode::idReg_t rsDepuisChaine(const char *instruction);
    
    codage::instruction_t  instructionDepuisChaine(const char *instruction);

    bool instructionEstValide();
}

#endif