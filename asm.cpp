#include "asm.h"

namespace asmb
{
    void erreur()
    {
        // on signale une erreur en mettant le bit de poids faible a 1
        g_instruction |= opcode::drapeau::ERR;
    }

    char *nettoyerEspacesChaine(const char *instruction)
    {
        static char instructionPropre[9] = { 0 };
        char *indiceChaineSource      { (char *) instruction };
        char *indiceChaineDestination { instructionPropre };

        while (*indiceChaineSource)
        {
            if (*indiceChaineSource != ' ')
            {
                *indiceChaineDestination = *indiceChaineSource;
                ++indiceChaineDestination;
            }

            ++indiceChaineSource;
        }

        return instructionPropre;
    }

    int numRegistreDepuisCaractere(const char caractere)
    {
        if (caractere <= '7' && caractere >= '0')
            return static_cast<int>(caractere - 48);

        switch (caractere)
        {
            case 'n' :
            case 'o' :
                return 0;
            case 'x' :
                return 1;
            case 'y' :
                return 2;
            case 'z' :
                return 3;
            case 'i' :
                return 4;
            case 'a' :
                return 5;
            case 'p' :
                return 6;
            case 's' :
                return 7;
        }

        erreur();
        return -1;
    }

    opcode::reg_t rdDepuisChaine(const char *instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[1]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre << 6;
    }

    opcode::reg_t rtDepuisChaine(const char *instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[6]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre << 3;
    }

    opcode::reg_t rsDepuisChaine(const char *instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[8]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre;
    }

    opcode_t opcodeDepuisChaine(const char *instruction)
    {
        /*
         * ici on recupere l'opcode de l'instruction,
         * exemple :
         * .3 add .5 .6
         * on recupere d'abord add
         * puis on envoie un opcode qui va selon l'instruction
         */

        char operation[3] = { '\0' };
        strncpy(operation, instruction + 2, 3);

        if (!strcmp(operation, "add"))
            return opcode::ADD;
        if (!strcmp(operation, "dif"))
            return opcode::DIF;
        if (!strcmp(operation, "mul"))
            return opcode::MUL;
        if (!strcmp(operation, "div"))
            return opcode::DIV;
        if (!strcmp(operation, "mod"))
            return opcode::MOD;

        if (!strcmp(operation, "etl"))
            return opcode::ETL;
        if (!strcmp(operation, "oul"))
            return opcode::OUL;
        if (!strcmp(operation, "xor"))
            return opcode::XOR;
        if (!strcmp(operation, "nsi"))
            return opcode::NSI;
        if (!strcmp(operation, "nsd"))
            return opcode::NSD;

        if (!strcmp(operation, "cmm"))
            return opcode::CMM;
        if (!strcmp(operation, "com"))
            return opcode::COM;
        if (!strcmp(operation, "rmm"))
            return opcode::RMM;
        if (!strcmp(operation, "rom"))
            return opcode::ROM;

        erreur();
        return opcode::NUL;
    }

    instruction_t instructionDepuisChaine(const char *instruction)
    {
        opcode_t op = opcodeDepuisChaine(instruction);
        opcode::reg_t rd = rdDepuisChaine(instruction);
        opcode::reg_t rt = rtDepuisChaine(instruction);
        opcode::reg_t rs = rsDepuisChaine(instruction);

        g_instruction |= (op | rd | rt | rs);

        return g_instruction;
    }

    bool instructionValide()
    {
        return !(g_instruction & 1);
    }
}

