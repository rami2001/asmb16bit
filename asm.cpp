#include "asm.h"

namespace asmb
{
    std::string chaineFormelle(const std::string &instruction)
    {
        std::string chaine = instruction;
        
        // pas la peine de se casser la tete, il suffit d'utiliser la bibliotheque standard
        chaine.erase(std::remove_if(chaine.begin(), chaine.end(), ::isspace), chaine.end());

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

        return ERR;
    }

    codage::opcode::idReg_t rdDepuisChaine(const std::string &instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[1]);

        if (numRegistre == ERR)
            return ERR;
        else
            return numRegistre << 7;
    }

    codage::opcode::idReg_t rtDepuisChaine(const std::string &instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[6]);

        if (numRegistre == ERR)
            return ERR;
        else
            return numRegistre << 4;
    }

    codage::opcode::idReg_t rsDepuisChaine(const std::string &instruction)
    {
        int numRegistre = numRegistreDepuisCaractere(instruction[8]);

        if (numRegistre == ERR)
            return ERR;
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

        return ERR;
    }

    codage::instruction_t instructionDepuisChaine(const std::string &instruction)
    {
        codage::instruction_t codeMachine = { 0 };
    
        codage::opcode_t op = opcodeDepuisChaine(instruction);

        if (op == codage::opcode::EXE)
            return codage::opcode::EXE;

        codage::opcode::idReg_t rd = rdDepuisChaine(instruction);
        codage::opcode::idReg_t rt = rtDepuisChaine(instruction);
        codage::opcode::idReg_t rs = rsDepuisChaine(instruction);

        if (rd == ERR || rt == ERR || rs == ERR || op == ERR)
            return codeMachine & codage::masque::ERREUR; // bit de poids faible directement a 1

        codeMachine = op | rd | rt | rs;

        return codeMachine;
    }
}

