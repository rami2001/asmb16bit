#include "Cpu.h"

namespace asmb
{
    Cpu::Cpu()
    {
        for (int i = 0; i < 8; ++i)
            Cpu::m_registre[i] = 0;

        Cpu::m_compteurOridnal     = 0x8000;
        Cpu::m_registreInstruction = 0;

        for (std::size_t i = 0; i < 0xffff; ++i)
            m_memoire[i] = 0;
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
        if (instruction & codage::masque::ERREUR)
            exit(EXIT_FAILURE);

        codage::opcode_t opcode    { static_cast<codage::opcode_t>(instruction & codage::masque::OPCODE) };

        codage::opcode::idReg_t rd { static_cast<codage::opcode::idReg_t>(instruction & codage::masque::RD) };
        codage::opcode::idReg_t rt { static_cast<codage::opcode::idReg_t>(instruction & codage::masque::RT) };
        codage::opcode::idReg_t rs { static_cast<codage::opcode::idReg_t>(instruction & codage::masque::RS) };

        if (opcode == codage::opcode::EXE)
        {
            exe();
            return;
        }

        int numRd = rd >> 7;
        int numRt = rt >> 4;
        int numRs = rs >> 1;

        constexpr codage::opcode_t EST_ARITHMETIQUE { 0x4000 };

        /*
         * opcode des operations arithmetiques commence par (01xxxx)
         * 0xc000 permet de recuperer les deux premiers bits et les comparer avec 0x4000
         */

        if ((opcode & 0xc000) == EST_ARITHMETIQUE)
        {
            switch (opcode)
            {
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
            }
        }

        constexpr codage::opcode_t EST_LOGIQUE { 0x2000 };

        /*
         * opcode des operations arithmetiques commence par (001xxx)
         * 0xe000 permet de recuperer les trois premiers bits et les comparer avec 0x2000
         */
        if ((opcode & 0xe000) == EST_LOGIQUE)
        {
            switch(opcode)
            {
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

        constexpr codage::opcode_t EST_MEMOIRE { 0x6000 };

        /*
         * opcode des operations arithmetiques commence par (011xxx)
         * 0xe000 permet de recuperer les trois premiers bits et les comparer avec 0x2000
         */

        if ((opcode & 0xe000) == EST_MEMOIRE)
        {
            switch(opcode)
            {
                case codage::opcode::CMM :
                    cmm(numRd, numRt, numRs);
                    break;
                case codage::opcode::COM :
                    com(numRd, numRt, numRs);
                    break;
                case codage::opcode::RMM :
                    rmm(numRd, numRt, numRs);
                    break;
                case codage::opcode::ROM :
                    rom(numRd, numRt, numRs);
                    break;
            }
        }
    }

    void Cpu::exec(const std::string &instruction)
    {
        codage::instruction_t instructionExecutable = instructionDepuisChaine(chaineFormelle(instruction));

        exec(instructionExecutable);
    }

    void Cpu::add(const int& rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] + Cpu::m_registre[rs];
    }

    void Cpu::dif(const int& rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] - Cpu::m_registre[rs];
    }

    void Cpu::mul(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] * Cpu::m_registre[rs];
    }

    void Cpu::div(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
        {
            if (Cpu::m_registre[rs] == 0)
                exit(EXIT_FAILURE);
            else
                Cpu::m_registre[rd] = Cpu::m_registre[rt] / Cpu::m_registre[rs];
        }
    }

    void Cpu::mod(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
        {
            if (Cpu::m_registre[rs] == 0)
                exit(EXIT_FAILURE);
            else
                Cpu::m_registre[rd] = Cpu::m_registre[rt] % Cpu::m_registre[rs];
        }
    }

    void Cpu::etl(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] & Cpu::m_registre[rs];
    }

    void Cpu::oul(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] | Cpu::m_registre[rs];
    }

    void Cpu::oux(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] ^ Cpu::m_registre[rs];
    }

    void Cpu::nsd(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] == Cpu::m_registre[rs];
    }

    void Cpu::nsi(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_registre[rt] >= Cpu::m_registre[rs];
    }

    void Cpu::cmm(const int &rd, const int &rt, const int &rs)
    {
        if (rd)
            Cpu::m_registre[rd] = Cpu::m_memoire[m_registre[rt] + m_registre[rs]];
    }

    void Cpu::com(const int &rd, const int &rt, const int &rs)
    {
        // 0x0011 pour recuperer seulement les 08 derniers bits
        if (rd)
            Cpu::m_registre[rd] = 0x0011 & Cpu::m_memoire[m_registre[rt] + m_registre[rs]];
    }

    void Cpu::rmm(const int &rd, const int &rt, const int &rs)
    {
        Cpu::m_memoire[Cpu::m_registre[rt] + Cpu::m_registre[rs]] = Cpu::m_memoire[rd];
    }

    void Cpu::rom(const int &rd, const int &rt, const int &rs)
    {
        // ranger seulement les 08 bits de poids faible
        Cpu::m_memoire[Cpu::m_registre[rt] + Cpu::m_registre[rs]] &= 0x1100;
        reg_t temp = m_registre[rd] & 0x0011;
        Cpu::m_memoire[Cpu::m_registre[rt] + Cpu::m_registre[rs]] = temp;
    }

    const char *Cpu::ecrireChaine(const int &longueur)
    {
        static char *chaine;

        int i { 0 };
        for (i = 0; i < longueur; ++i)
            *(chaine + i) = m_memoire[m_registre[5] + i];

        *(chaine + i + 1) = { '\0' };

        return chaine;
    }

    void Cpu::exe()
    {
        // depend selon la valeur du registre 07
        switch (m_registre[7])
        {
            case 0 :
                std::cout << "Programme execute avec succes";
                break;
            case 1 :
                std::cout << Cpu::m_registre[6] << '\n';
                break;
            case 2 :
                Cpu::ecrireChaine(Cpu::m_registre[6]);
                break;
            case 3 :
                std::cin >> Cpu::m_registre[6];
                break;
            default :
                exit(EXIT_FAILURE);
                break;
        }
    }
}
