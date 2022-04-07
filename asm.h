#ifndef ASM_H
#define ASM_H

#include "opcodes.h"

#include <cstring>

namespace asmb
{
    static instruction_t g_instruction { 0 };

    void erreur(void);

    char *nettoyerEspacesChaine(const char* instruction);

    opcode_t opcodeDepuisChaine(const char *instruction);
    int numRegistreDepuisCaractere(const char caractere);

    opcode::reg_t rdDepuisChaine(const char *instruction);
    opcode::reg_t rtDepuisChaine(const char *instruction);
    opcode::reg_t rsDepuisChaine(const char *instruction);
    
    instruction_t  instructionDepuisChaine(const char *instruction);

    bool instructionValide();
}

#endif