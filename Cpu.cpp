#include "Cpu.h"

#include <cstdlib>
#include <iostream>

namespace asmb
{
    Cpu::Cpu()
    {
        for (int i = 0; i < 8; ++i)
            Cpu::m_registre[i] = 0;

        Cpu::m_compteurOridnal     = 0x8000;
        Cpu::m_registreInstruction = 0;

        memset(Cpu::m_memoire, 0, 0xffff);
    }

    reg_t Cpu::getReg(const int &num)
    {
        return Cpu::m_registre[num];
    }

    void Cpu::setReg(const int &index, const reg_t &val)
    {
        Cpu::m_registre[index] = val;
    }

    void Cpu::exec(codage::instruction_t instruction)
    {
        if (instruction & codage::masque::ERR)
            exit(EXIT_FAILURE);

        codage::opcode_t opcode    { static_cast<codage::opcode_t>(instruction & codage::masque::OPCODE) };

        codage::opcode::idReg_t rd { static_cast<codage::opcode::idReg_t>(instruction & codage::masque::RD) };
        codage::opcode::idReg_t rt { static_cast<codage::opcode::idReg_t>(instruction & codage::masque::RT) };
        codage::opcode::idReg_t rs { static_cast<codage::opcode::idReg_t>(instruction & codage::masque::RS) };

        int numRd = rd >> 7;
        int numRt = rt >> 4;
        int numRs = rs >> 1;

        switch (opcode)
        {
            // instructions d'arithmetique
            case codage::opcode::ADD :
                add(numRd, numRt, numRs);
                break;
            case codage::opcode::DIF :
                dif(numRd, numRt, numRs);
                break;
            case codage::opcode::MUL :
                mul(numRd, numRt, numRs);
                break;
            case codage::opcode::DIV :
                div(numRd, numRt, numRs);
                break;
            case codage::opcode::MOD :
                mod(numRd, numRt, numRs);
                break;

            // instructions logiques
            case codage::opcode::ETL :
                etl(numRd, numRt, numRs);
                break;
            case codage::opcode::OUL :
                oul(numRd, numRt, numRs);
                break;
            case codage::opcode::XOR :
                oux(numRd, numRt, numRs);
                break;
            case codage::opcode::NSI :
                nsi(numRd, numRt, numRs);
                break;
            case codage::opcode::NSD :
                nsd(numRd, numRt, numRs);
                break;
        }
    }

    void Cpu::exec(const char *instruction)
    {
        char *instructionPropre = chaineFormelle(instruction);

        codage::instruction_t instructionExecutable = instructionDepuisChaine(instructionPropre);

        exec(instructionExecutable);
    }

    void Cpu::add(const int& rd, const int &rt, const int &rs)
    {
        if (rd != 0)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] + Cpu::m_registre[rs];
    }

    void Cpu::dif(const int& rd, const int &rt, const int &rs)
    {
        if (rd != 0)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] - Cpu::m_registre[rs];
    }

    void Cpu::mul(const int &rd, const int &rt, const int &rs)
    {
        if (rd != 0)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] * Cpu::m_registre[rs];
    }

    void Cpu::div(const int &rd, const int &rt, const int &rs)
    {
        if (rd != 0)
        {
            if (Cpu::m_registre[rs] == 0)
                exit(EXIT_FAILURE);
            else
                Cpu::m_registre[rd] = Cpu::m_registre[rt] / Cpu::m_registre[rs];
        }
    }

    void Cpu::mod(const int &rd, const int &rt, const int &rs)
    {
        if (rd != 0)
        {
            if (Cpu::m_registre[rs] == 0)
                exit(EXIT_FAILURE);
            else
                Cpu::m_registre[rd] = Cpu::m_registre[rt] % Cpu::m_registre[rs];
        }
    }

    void Cpu::etl(const int &rd, const int &rt, const int &rs)
    {
        if (rd != 0)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] & Cpu::m_registre[rs];
    }

    void Cpu::oul(const int &rd, const int &rt, const int &rs)
    {
        if (rd != 0)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] | Cpu::m_registre[rs];
    }

    void Cpu::oux(const int &rd, const int &rt, const int &rs)
    {
        if (rd != 0)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] ^ Cpu::m_registre[rs];
    }

    void Cpu::nsd(const int &rd, const int &rt, const int &rs)
    {
        if (rd != 0)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] == Cpu::m_registre[rs];
    }

    void Cpu::nsi(const int &rd, const int &rt, const int &rs)
    {
        if (rd != 0)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] >= Cpu::m_registre[rs];
    }
}
