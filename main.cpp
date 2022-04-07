#include "Cpu.h"

#include <iostream>
#include <bitset>

int main(int argc, char const *argv[])
{
    char *ins = asmb::nettoyerEspacesChaine("   .3 add   .6  .7");

    std::bitset<16> instruction = asmb::instructionDepuisChaine(ins);

    std::cout << instruction;

    return 0;
}
