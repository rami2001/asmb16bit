#ifndef OPCODES_H
#define OPCODES_H

#include "types.h"

/*
 * les opcodes sont sous la forme :
 *
 * OPCODE | RD | RT | RS | BIT ERREUR
 *
 * opcode sur 06 bits,
 * rd sur 03 bits;
 * rt sur 03 bits;
 * rs sur 03 bits;
 *
 */

namespace asmb
{
    namespace codage
    {
        namespace opcode
        {
            /*
            *  les opcodes se representent sur les 06 bits de poids faible (0b'0000'0000'0000'000)
            */

            inline constexpr opcode_t NUL { 0b0000'0000'0000'000 };

            // les opcodes des operations arithmetiques (01xxxx)
            inline constexpr opcode_t ADD { 0b0100'0000'0000'0000 };
            inline constexpr opcode_t DIF { 0b0100'0100'0000'0000 };
            inline constexpr opcode_t MUL { 0b0100'1000'0000'0000 };
            inline constexpr opcode_t DIV { 0b0100'1100'0000'0000 };
            inline constexpr opcode_t MOD { 0b0101'0000'0000'0000 };

            // les opcodes des operations logiques      (001xxx)
            inline constexpr opcode_t ETL { 0b0010'0000'0000'0000 };
            inline constexpr opcode_t OUL { 0b0010'0100'0000'0000 };
            inline constexpr opcode_t XOR { 0b0010'1000'0000'0000 };
            inline constexpr opcode_t NSI { 0b0010'1100'0000'0000 };
            inline constexpr opcode_t NSD { 0b0011'0000'0000'0000 };

            // les opcodes des operations sur la memoire (011xxx)
            inline constexpr opcode_t CMM { 0b0110'0000'0000'0000 };
            inline constexpr opcode_t COM { 0b0110'0010'0000'0000 };
            inline constexpr opcode_t RMM { 0b0110'0100'0000'0000 };
            inline constexpr opcode_t ROM { 0b0110'0110'0000'0000 };

            // les opcodes des operations sur les bits   (100xxx)
            inline constexpr opcode_t CBG { 0b1000'0000'0000'0000 };
            inline constexpr opcode_t DGL { 0b1000'0100'0000'0000 };
            inline constexpr opcode_t DDL { 0b1000'1000'0000'0000 };
            inline constexpr opcode_t CBD { 0b1001'1100'0000'0000 };

            // les opcodes des operations de branchement  (101xxx)
            inline constexpr opcode_t ALL { 0b1010'0000'0000'0000 };
            inline constexpr opcode_t SEG { 0b1010'0100'0000'0000 };
            inline constexpr opcode_t SIS { 0b1010'1000'0000'0000 };
            inline constexpr opcode_t SIE { 0b1010'1100'0000'0000 };

            // l'opcode exe de l'appel systeme            (111111)
            inline constexpr opcode_t EXE { 0b1111'1100'0000'0000 };
        }

        namespace masque
        {
            // les masques de bits des registres
            inline constexpr instruction_t RD { 0b0000'0011'1000'0000 };
            inline constexpr instruction_t RT { 0b0000'0000'0111'0000 };
            inline constexpr instruction_t RS { 0b0000'0000'0000'1110 };

            // le masque de l'opcode
            inline constexpr instruction_t OPCODE { 0b1111'1100'0000'0000 };

            // le dernier bit designe une erreur
            inline constexpr instruction_t ERREUR { 1 };
        }
    }
}

#endif