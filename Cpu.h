#ifndef CPU_H
#define CPU_H

#include "asm.h"

namespace asmb
{
    class Cpu
    {
        private :
            reg_t m_registre[8];
            reg_t m_compteurOridnal;
            instruction_t m_registreInstruction;
            mot_t m_memoire[0xffff];

        public :
            Cpu();
    };    
}

#endif
