#include "asm.h"

namespace asmb
{
    void erreur()
    {
        // on signale une erreur en mettant le bit de poids faible a 1
        g_instruction |= codage::masque::ERR;
    }

    bool instructionEstValide()
    {
        return !(g_instruction & 1);
    }

    char *chaineFormelle(const char *instruction)
    {
        static char instructionPropre[10] = { 0 };

        char *indiceChaineSource      { (char *) instruction };
        char *indiceChaineDestination { instructionPropre };

        while (*indiceChaineSource)
        {
            if (*indiceChaineSource != ' ' && *indiceChaineSource != '\t')
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

    codage::opcode::idReg_t rdDepuisChaine(const char *instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[1]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre << 7;
    }

    codage::opcode::idReg_t rtDepuisChaine(const char *instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[6]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre << 4;
    }

    codage::opcode::idReg_t rsDepuisChaine(const char *instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[8]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre << 1;
    }

    codage::opcode_t opcodeDepuisChaine(const char *instruction)
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

        if (!strcmp(operation, "etl"))
            return codage::opcode::ETL;
        if (!strcmp(operation, "oul"))
            return codage::opcode::OUL;
        if (!strcmp(operation, "xor"))
            return codage::opcode::XOR;
        if (!strcmp(operation, "oux"))
            return codage::opcode::XOR;
        if (!strcmp(operation, "nsd"))
            return codage::opcode::NSD;
        if (!strcmp(operation, "nsi"))
            return codage::opcode::NSI;

        if (!strcmp(operation, "add"))
            return codage::opcode::ADD;
        if (!strcmp(operation, "dif"))
            return codage::opcode::DIF;
        if (!strcmp(operation, "mul"))
            return codage::opcode::MUL;
        if (!strcmp(operation, "div"))
            return codage::opcode::DIV;
        if (!strcmp(operation, "mod"))
            return codage::opcode::MOD;

        if (!strcmp(operation, "cmm"))
            return codage::opcode::CMM;
        if (!strcmp(operation, "com"))
            return codage::opcode::COM;
        if (!strcmp(operation, "rmm"))
            return codage::opcode::RMM;
        if (!strcmp(operation, "rom"))
            return codage::opcode::ROM;

        if (!strcmp(operation, "cbg"))
            return codage::opcode::CBG;
        if (!strcmp(operation, "dgl"))
            return codage::opcode::DGL;
        if (!strcmp(operation, "ddl"))
            return codage::opcode::DDL;
        if (!strcmp(operation, "cbd"))
            return codage::opcode::CBD;

        if (!strcmp(operation, "all"))
            return codage::opcode::ALL;
        if (!strcmp(operation, "seg"))
            return codage::opcode::SEG;
        if (!strcmp(operation, "sis"))
            return codage::opcode::SIS;
        if (!strcmp(operation, "sie"))
            return codage::opcode::SIE;

        if (!strcmp(operation, "exe"))
            return codage::opcode::EXE;

        erreur();
        return codage::opcode::NUL;
    }

    codage::instruction_t instructionDepuisChaine(const char *instruction)
    {
        codage::opcode_t op        = opcodeDepuisChaine(instruction);
        codage::opcode::idReg_t rd = rdDepuisChaine(instruction);
        codage::opcode::idReg_t rt = rtDepuisChaine(instruction);
        codage::opcode::idReg_t rs = rsDepuisChaine(instruction);

        g_instruction |= (op | rd | rt | rs);

        return g_instruction;
    }
}

