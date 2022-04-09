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

    std::string chaineFormelle(const std::string &instruction)
    {
        std::string chaine = instruction;

        remove(chaine.begin(), chaine.end(), ' ');

        return chaine;
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

    codage::opcode::idReg_t rdDepuisChaine(const std::string &instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[1]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre << 7;
    }

    codage::opcode::idReg_t rtDepuisChaine(const std::string &instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[6]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre << 4;
    }

    codage::opcode::idReg_t rsDepuisChaine(const std::string &instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[8]);

        if (numRegistre == -1)
            erreur();
        else
            return numRegistre << 1;
    }

    codage::opcode_t opcodeDepuisChaine(const std::string &instruction)
    {
        /*
         * ici on recupere l'opcode de l'instruction,
         * exemple :
         * .3 add .5 .6
         * on recupere d'abord add
         * puis on envoie un opcode qui va selon l'instruction
         */

        if (instruction == "exe")
            return codage::opcode::EXE;

        std::string operation = instruction.substr(2, 3);

        if (operation == "etl")
            return codage::opcode::ETL;
        if (operation == "oul")
            return codage::opcode::OUL;
        if (operation == "xor")
            return codage::opcode::XOR;
        if (operation == "oux")
            return codage::opcode::XOR;
        if (operation == "nsd")
            return codage::opcode::NSD;
        if (operation == "nsi")
            return codage::opcode::NSI;

        if (operation == "add")
            return codage::opcode::ADD;
        if (operation == "dif")
            return codage::opcode::DIF;
        if (operation == "mul")
            return codage::opcode::MUL;
        if (operation == "div")
            return codage::opcode::DIV;
        if (operation == "mod")
            return codage::opcode::MOD;

        if (operation == "cmm")
            return codage::opcode::CMM;
        if (operation == "com")
            return codage::opcode::COM;
        if (operation == "rmm")
            return codage::opcode::RMM;
        if (operation == "rom")
            return codage::opcode::ROM;

        if (operation == "cbg")
            return codage::opcode::CBG;
        if (operation == "dgl")
            return codage::opcode::DGL;
        if (operation == "ddl")
            return codage::opcode::DDL;
        if (operation == "cbd")
            return codage::opcode::CBD;

        if (operation == "all")
            return codage::opcode::ALL;
        if (operation == "seg")
            return codage::opcode::SEG;
        if (operation == "sis")
            return codage::opcode::SIS;
        if (operation == "sie")
            return codage::opcode::SIE;

        if (operation == "exe")
            return codage::opcode::EXE;

        erreur();
        return codage::opcode::NUL;
    }

    codage::instruction_t instructionDepuisChaine(const std::string &instruction)
    {
    	// reinitialiser valeur de l'instruction sauf le bit d'erreur
        g_instruction &= 1;
    
        codage::opcode_t op = opcodeDepuisChaine(instruction);

        if (op == codage::opcode::EXE)
            return codage::opcode::EXE;

        codage::opcode::idReg_t rd = rdDepuisChaine(instruction);
        codage::opcode::idReg_t rt = rtDepuisChaine(instruction);
        codage::opcode::idReg_t rs = rsDepuisChaine(instruction);

        g_instruction |= (op | rd | rt | rs);

        return g_instruction;
    }
}

