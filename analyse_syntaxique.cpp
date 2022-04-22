#include "analyse_syntaxique.h"

/*
    axiome : <axiome>

    <axiome>         -> <label> <suite label> | <registre> <suite registre> | <label_ligne> | <appel systeme>
    <suite label>    -> <separateur> | all | <instruction branchement> <registre> <registre>
    <suite registre> -> all | <instructino normale> <registre> <registre> | cbg <immediat> | cbd <immediat>
*/

namespace asmb
{
    opcode::opcode_t opcode_depuis_token(const Token &token)
    {
        std::string valeur = token.literal;

        if (valeur == "add")
            return opcode::ADD;

        if (valeur == "dif")
            return opcode::DIF;

        if (valeur == "mul")
            return opcode::MUL;

        if (valeur == "div")
            return opcode::DIV;

        if (valeur == "mod")
            return opcode::MOD;



        if (valeur == "etl")
            return opcode::ETL;

        if (valeur == "oul")
            return opcode::OUL;

        if (valeur == "xor")
            return opcode::XOR;

        if (valeur == "nsi")
            return opcode::NSI;

        if (valeur == "nsd")
            return opcode::NSD;



        if (valeur == "cmm")
            return opcode::CMM;

        if (valeur == "com")
            return opcode::COM;

        if (valeur == "rmm")
            return opcode::RMM;

        if (valeur == "rom")
            return opcode::ROM;



        if (valeur == "cbg")
            return opcode::CBG;

        if (valeur == "cbd")
            return opcode::CBD;

        if (valeur == "dgl")
            return opcode::DGL;

        if (valeur == "ddl")
            return opcode::DDL;

        if (valeur == "all")
            return opcode::ALL;

        if (valeur == "seg")
            return opcode::SEG;

        if (valeur == "sis")
            return opcode::SIS;

        if (valeur == "sie")
            return opcode::SIE;
    }

    Token defiler(std::queue<Token> &file)
    {
        Token token = file.front();
        file.pop();
        
        return token;
    }

    opcode::instruction_t cbg(const int &numero_registre, const int &immediat)
    {
        opcode::instruction_t instruction { 0 };

        instruction |= opcode::CBG;
        instruction |= numero_registre << 8;
        instruction |= (immediat & 0xff00) >> 2;

        return instruction;
    }

    opcode::instruction_t cbd(const int &numero_registre, const int &immediat)
    {
        opcode::instruction_t instruction { 0 };

        instruction |= opcode::CBD;
        instruction |= numero_registre << 8;
        instruction |= immediat & 0x00ff;

        return instruction;
    }
    
    opcode::id_reg_t rd(const int &numero_registre)
    {
        return numero_registre << 7;   
    }

    opcode::id_reg_t rt(const int &numero_registre)
    {
        return numero_registre << 4;   
    }

    opcode::id_reg_t rs(const int &numero_registre)
    {
        return numero_registre << 1;   
    }

    reg_t *analyse_syntaxique(std::queue<Token> &file_tokens)
    {
        int          ptr_memoire { 0 };
        static reg_t programme[0x8000] = { 0 };
        
        std::unordered_map<std::string, int> labels;

        while (!file_tokens.empty())
        {
            Token token = defiler(file_tokens);
            
            // exe
            if (token.type == INSTRUCTION && token.valeur == APPEL_SYSTEME)
            {
                programme[ptr_memoire] = opcode::EXE;
                ++ptr_memoire;
                continue;
            }

            if (token.type == LABEL_LIGNE)
            {
                labels[token.literal] = ptr_memoire;
                continue;
            }

            // instruction qui commence par un label
            if (token.type == LABEL)
            {   
                std::string label = token.literal;
                token = defiler(file_tokens);
                // on doit s'attendre soit a :

                // separateur (:)
                if (token.type == SEPARATEUR)
                {
                    labels[token.literal] = ptr_memoire;
                    continue;
                }

                // instruction label
                if (token.type == INSTRUCTION && token.valeur == BRANCHEMENT)
                {
                    // on verifie si le label existe
                    if (labels.find(label) != labels.end())
                    {
                        // ok il existe
                        // on charge le registre 05 (registre adresse)
                        programme[ptr_memoire] = cbg(5, labels[label]);
                        ++ptr_memoire;
                            
                        programme[ptr_memoire] = cbd(5, labels[label]);
                        ++ptr_memoire;

                        if (token.literal == "all")
                        {
                            programme[ptr_memoire] = opcode::ALL | rd(5);
                            ++ptr_memoire;

                            continue;
                        }

                        // on recupere l'opcode
                        opcode::opcode_t op = opcode_depuis_token(token);

                        // on doit s'attendre a deux registres
                        token = defiler(file_tokens);
                        
                        if (token.type == REGISTRE)
                        {
                            // ok
                            opcode::instruction_t rt_instruction = rt(token.valeur);
                            
                            // on doit s'attendre a un autre registre
                            token = defiler(file_tokens);

                            if (token.type == REGISTRE)
                            {
                                // ok ! label, branchement, rt, rs
                                opcode::instruction_t rs_instruction = rs(token.valeur);

                                // on execute l'instruction avec le registre 5
                                programme[ptr_memoire] = op | rd(5) | rt_instruction | rs_instruction;
                                ++ptr_memoire;

                                continue;
                            }
                            else
                            {
                                std::cout << "Erreur, deux registres attendus apres l'instruction de branchement";
                                exit(EXIT_FAILURE);
                            }
                        }
                        else
                        {
                            std::cout << "Erreur, registre attendu apres l'instruction de branchement";
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        std::cout << "Erreur, le label '" << label << "' n'existe pas\n";
                        exit(EXIT_FAILURE);
                    }                    
                }
                else
                {
                    std::cout << "Erreur, instruction ou separateur attendu apres le label '" << label << '\'' << '\n';
                    exit(EXIT_FAILURE);
                }
            }

            // instruction qui commence par un registre
            if (token.type == REGISTRE)
            {
                opcode::instruction_t rd_instruction = rd(token.valeur);

                // on avance et on doit s'attendre a une instruction normale, ou cbg/cbd
                token = defiler(file_tokens);

                if (token.type == INSTRUCTION )
                {
                    if (token.valeur == CB)
                    {
                        bool estCBG = (opcode_depuis_token(token) == opcode::CBG);

                        // on doit s'attendre a un immediat
                        token = defiler(file_tokens);

                        if (token.type == IMMEDIAT)
                        {
                            // ok !
                            if (estCBG)
                                programme[ptr_memoire] = cbg(rd_instruction, token.valeur);
                            else
                                programme[ptr_memoire] = cbd(rd_instruction, token.valeur);

                            ++ptr_memoire;
                            continue;
                        }
                        else
                        {
                            std::cout << "Erreur, immediat attendu apres 'cbg' ou 'cbd' !\n";
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if (token.valeur == NORMALE)
                    {
                        // on recupere l'instruction
                        opcode::opcode_t op = opcode_depuis_token(token);

                        // on doit s'attendre a deux registres
                        token = defiler(file_tokens);

                        if (token.type == REGISTRE)
                        {
                            // ok
                            opcode::instruction_t rt_instruction = rt(token.valeur);
                            
                            // on doit s'attendre a un autre registre ou un immediat
                            token = defiler(file_tokens);

                            if (token.type == REGISTRE)
                            {
                                // ok ! rd, instruction, rt, rs
                                opcode::instruction_t rs_instruction = rs(token.valeur);

                                // on execute l'instruction avec le registre 5
                                programme[ptr_memoire] = op | rd_instruction | rt_instruction | rs_instruction;
                                ++ptr_memoire;

                                continue;
                            }
                            else if (token.type == IMMEDIAT)
                            {
                                programme[ptr_memoire] = cbg(4, token.valeur);
                                ++ptr_memoire;

                                programme[ptr_memoire] = cbd(4, token.valeur);
                                ++ptr_memoire;

                                // on execute l'instruction avec le registre 4
                                programme[ptr_memoire] = op | rd_instruction | rt_instruction | rs(4);
                                ++ptr_memoire;

                                continue;
                            }
                            else
                            {
                                std::cout << "Erreur, registre ou immediat attendu en derniere operande\n";
                                exit(EXIT_FAILURE);
                            }
                        }
                        else
                        {
                            std::cout << "Erreur, registre attendu apres l'instruction\n";
                            exit(EXIT_FAILURE);
                        }

                    }
                    else
                    {
                        std::cout << "Erreur, instruction incorrecte apres registre !\n";
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    std::cout << "Erreur, instruction attendue apres registre !\n";
                    exit(EXIT_FAILURE);
                }
            }

            std::cout << "Erreur, '" << token.literal << "' non attendu !\n";
            exit(EXIT_FAILURE);
        }

        programme[ptr_memoire] = cbg(7, 0);
        ++ptr_memoire;

        programme[ptr_memoire] = cbd(7, 0);
        ++ptr_memoire;

        programme[ptr_memoire] = opcode::EXE;
        ++ptr_memoire;

        std::bitset<16> code_machine = 0;

        int i = 0;
        while (programme[i] != 0)
        {
            std::cout << code_machine << std::endl;
            ++i;
        }

        return programme;
    }
}
