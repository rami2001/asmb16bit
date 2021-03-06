#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace asmb
{   
    typedef std::uint16_t ns_mot_t;
    typedef std::int16_t  mot_t;
    typedef std::int16_t  reg_t;
    typedef std::int8_t   octet_t;

    namespace opcode
    {
        typedef std::uint16_t opcode_t;
        typedef std::uint16_t instruction_t;
        typedef std::uint16_t id_reg_t;
    }
}

#endif