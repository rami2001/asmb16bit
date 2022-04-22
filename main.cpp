#include "analyse_syntaxique.h"

#include <bitset>

int main(int argc, char const *argv[])
{
    std::queue<asmb::Token> file_tokens = asmb::analyse_lexicale("asmb.txt");

    asmb::reg_t* programme = asmb::analyse_syntaxique(file_tokens);

    return 0;
}
