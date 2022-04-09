#include "Cpu.h"

#include <iostream>
#include <bitset>

int main(int argc, char const *argv[])
{
    asmb::Cpu cpu;

    cpu.setReg(1, 1);
    cpu.setReg(6, 1);
    cpu.setReg(7, 1); // reinitialise a 1 pour afficher registre 06

    cpu.exec(".6 dgl .6 .1");
    cpu.exec("exe");

    cpu.exec(".6 dgl .6 .1");
    cpu.exec("exe");

    cpu.exec(".6 dgl .6 .1");
    cpu.exec("exe");

    cpu.setReg(7, 0);
    cpu.exec("exe");

    return 0;
}
