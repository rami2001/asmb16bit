#include "Cpu.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    asmb::Cpu cpu;

    cpu.setReg(6, 20);
    cpu.setReg(7, 1);

    cpu.exec(" .3   etl .6      .7");

    std::cout << cpu.getReg(3);

    return 0;
}
