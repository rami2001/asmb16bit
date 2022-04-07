#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace asmb
{   
    typedef std::int16_t  mot_t;
    typedef std::int16_t  reg_t;
    typedef std::uint16_t opcode_t;
    typedef std::uint16_t instruction_t;

    namespace opcode
    {   
        // pour identifier un registre par l'opcode
        typedef std::uint16_t reg_t;
    }
}

#endif