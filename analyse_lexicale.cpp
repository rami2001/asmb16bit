#include "analyse_lexicale.h"

namespace asmb
{
    Token::Token(const std::string &l, const type_token_e &t, const int &v)
    {
        literal = l;
        type    = t;
        valeur  = v;
    }
    
    Token token_depuis_chaine(const std::string &chaine)
    {
        std::regex registre_numero("(\\.|%)[0-7]");

        std::regex registre_lettre("(\\.|%)[noxyziaps]");
        std::regex immediat_decimal("#?(-?[0-9]+)");
        std::regex immediat_hex("#?(0x[0-9a-fA-F]+)");

        std::regex label("[a-zA-Z][a-zA-Z0-9_]*");
        std::regex label_ligne("[a-zA-Z][a-zA-Z0-9_]*:");

        std::regex instruction_branchement("(all|seg|sis|sie)");
        std::regex instruction_normale("(etl|oul|xor|nsd|nsi|add|dif|mul|div|mod|cmm|com|rmm|rom|dgl|ddl)");
        std::regex instruction_cb("(cbg|cbd)");

        if (std::regex_match(chaine, immediat_decimal))
        {
            if (chaine[0] == '#')
                return { chaine, IMMEDIAT, stoi(chaine.substr(1)) };
            else
                return { chaine, IMMEDIAT, stoi(chaine) };
        }

        if (std::regex_match(chaine, immediat_hex))
        {
            int valeur { 0 };
            std::stringstream ss;

            if (chaine[0] == '#')
                ss << std::hex << chaine.substr(1);
            else
                ss << std::hex << chaine;

            ss >> valeur;
            return { chaine, IMMEDIAT, valeur };
        }

        if (std::regex_match(chaine, registre_numero))
        {
            return { chaine, REGISTRE, int(chaine[1] - 48) };
        }

        if (std::regex_match(chaine, registre_lettre))
        {
            int numero { 0 };

            switch (chaine[1])
            {
                case 'n':
                case 'o':
                    numero = 0;
                    break;
                case 'x' :
                    numero = 1;
                    break;
                case 'y' :
                    numero = 2;
                    break;
                case 'z' :
                    numero = 3;
                    break;
                case 'i' :
                    numero = 4;
                    break;
                case 'a' :
                    numero = 5;
                    break;
                case 'p' :
                    numero = 6;
                    break;
                case 's' :
                    numero = 7;
                    break;
            }

            return { chaine, REGISTRE, numero };
        }

        if (std::regex_match(chaine, instruction_branchement))
            return { chaine, INSTRUCTION, BRANCHEMENT };

        if (std::regex_match(chaine, instruction_normale))
            return { chaine, INSTRUCTION, NORMALE };

        if (std::regex_match(chaine, instruction_cb))
            return { chaine, INSTRUCTION, CB };

        if (chaine == "exe")
            return { chaine, INSTRUCTION, APPEL_SYSTEME };

        if (std::regex_match(chaine, label))
            return { chaine, LABEL, int(0x8000) };

        if (std::regex_match(chaine, label_ligne))
            return { chaine, LABEL_LIGNE, int(0x8000) };

        if (chaine == ":")
            return {chaine, SEPARATEUR, 0};

        return { chaine, ERREUR, -1 };
    }
    
    std::queue<Token> analyse_lexicale(const char* nom_fichier)
    {
        std::ifstream fichier;
        fichier.open(nom_fichier);

        if (!fichier.is_open())
        {
            std::cout << "Erreur lors de la lecture du fichier (asmb.txt non ouvert)." << std::endl;
        }

        std::string unite_lexicale;
        std::queue<Token> file_tokens;

        while (fichier >> unite_lexicale)
        {
            asmb::Token token = asmb::token_depuis_chaine(unite_lexicale);

            if (token.type == ERREUR)
            {
                // token non reconnu ! on arrete tout
                std::cout << '\'' << unite_lexicale << '\'' << " non reconnue ! Erreur lors de la phase d'analyse lexicale.\n";
                exit(EXIT_FAILURE);
            }
            else
            {
                file_tokens.push(token);
            }
        }
    }
}
