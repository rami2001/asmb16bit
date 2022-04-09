#include "Cpu.h"

#include <iostream>
#include <bitset>

int main(int argc, char const *argv[])
{
    asmb::Cpu cpu;

    cpu.set_reg(1, 1);
    cpu.set_reg(6, 64);
    cpu.set_reg(7, 1); // reinitialise a 1 pour afficher registre 06

    cpu.exec(".6 ddl .6 .1");
    cpu.exec("exe");

    cpu.exec(".6 ddl .6 .1");
    cpu.exec("exe");

    cpu.exec(".6 ddl .6 .1");
    cpu.exec("exe");

    cpu.exec(".6 ddl .6 .1");
    cpu.exec("exe");

    cpu.exec(".6 ddl .6 .1");
    cpu.exec("exe");

    cpu.set_reg(7, 0);
    cpu.exec("exe");

    return 0;
}
