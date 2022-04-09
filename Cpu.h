#ifndef CPU_H
#define CPU_H

#include "asm.h"

#include <cstdlib>
#include <iostream>

namespace asmb
{
    class Cpu
    {
        private :
            reg_t m_registre[8];
            reg_t m_compteurOridnal;
            reg_t m_registreInstruction;
            mot_t m_memoire[0xffff];

        public :
            Cpu();

            reg_t getReg(const int &num);
            void  setReg(const int &index, const reg_t &val);

            void exec(codage::instruction_t instruction);
            void exec(const std::string &instruction);

            void add(const int& rd, const int &rt, const int &rs);
            void dif(const int& rd, const int &rt, const int &rs);
            void mul(const int& rd, const int &rt, const int &rs);
            void div(const int& rd, const int &rt, const int &rs);
            void mod(const int& rd, const int &rt, const int &rs);

            void etl(const int& rd, const int &rt, const int &rs);
            void oul(const int& rd, const int &rt, const int &rs);
            void oux(const int& rd, const int &rt, const int &rs);
            void nsi(const int& rd, const int &rt, const int &rs);
            void nsd(const int& rd, const int &rt, const int &rs);

            void cmm(const int& rd, const int &rt, const int &rs);
            void com(const int& rd, const int &rt, const int &rs);
            void rmm(const int& rd, const int &rt, const int &rs);
            void rom(const int& rd, const int &rt, const int &rs);

            void cbg(const int &rd, const mot_t &valeur);
            void dgl(const int &rd, const int &rt, const int &rs);
            void ddl(const int &rd, const int &rt, const int &rs);
            void cbd(const int &rd, const mot_t &valeur);

            void all(const int &rd, const int &rt);
            void seg(const int &rd, const int &rt, const int &rs);
            void sis(const int &rd, const int &rt, const int &rs);
            void sie(const int &rd, const int &rt, const int &rs);

            std::string ecrireChaine(const int& longueur);
            void exe();
    };
}

#endif
