#include "Cpu.h"

#include <cstdlib>

asmb::Cpu::Cpu()
{
    Cpu::m_compteurOridnal     = 0x8000;
    Cpu::m_registreInstruction = 0;

    memset(Cpu::m_memoire, 0, 0xffff);

    for (int i = 0; i < 8; ++i)
        m_registre[i] = 0;
}
