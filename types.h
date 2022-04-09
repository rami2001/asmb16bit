#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace asmb
{   
    typedef std::int16_t  mot_t;
    typedef std::int16_t  reg_t;

    namespace codage
    {
        typedef std::uint16_t opcode_t;
        typedef std::uint16_t instruction_t;

        namespace opcode
        {
            typedef std::uint16_t idReg_t;
        }
    }
}

#endif