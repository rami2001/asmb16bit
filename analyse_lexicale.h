#ifndef ANALYSE_LEXICALE_H
#define ANALYSE_LEXICALE_H

#include "opcode.h"

#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <queue>

namespace asmb
{
    enum type_token_e       { REGISTRE, IMMEDIAT, LABEL, LABEL_LIGNE, INSTRUCTION, SEPARATEUR, ERREUR };
    enum type_instruction_e { BRANCHEMENT, CB, NORMALE, APPEL_SYSTEME };

    typedef struct Token
    {
        std::string  literal;
        type_token_e type;
        int          valeur; 

        Token(const std::string &l, const type_token_e &t, const int &v)
        {
            literal = "@";
            type    = ERREUR;
            valeur  = v;
        }
    } Token;

    std::queue<Token> analyse_lexicale(const char* nom_fichier);
    Token token_depuis_chaine(const std::string &chaine);
}

#endif