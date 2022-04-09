#include "Cpu.h"

#include <iostream>
#include <bitset>

int main(int argc, char const *argv[])
{
    asmb::Cpu cpu;

    cpu.setReg(3, 3);
    cpu.setReg(1, 4);
    cpu.setReg(7, 1);

    cpu.exec(".6 mul .3 .1");
    cpu.exec("exe");
    cpu.exec(".6 dif .6 .3");
    cpu.exec("exe");
    cpu.setReg(7, 0);
    cpu.exec("exe");

    return 0;
}
