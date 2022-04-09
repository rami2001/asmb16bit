#include "Cpu.h"

namespace asmb
{
    Cpu::Cpu()
    {
        for (int i = 0; i < 8; ++i)
            Cpu::m_registre[i] = 0;

        Cpu::m_compteur_oridnal     = 0x8000;
        Cpu::m_registre_instruction = 0;

        for (std::size_t i = 0; i < 0xffff; ++i)
            m_memoire[i] = 0;
    }

    reg_t Cpu::get_reg(const int &index)
    {
        return Cpu::m_registre[index];
    }

    void Cpu::set_reg(const int &index, const mot_t &val)
    {
        Cpu::m_registre[index] = val;
    }

    void Cpu::exec(const codage::instruction_t &instruction)
    {
        if (instruction & codage::masque::ERREUR)
            exit(EXIT_FAILURE);

        codage::opcode_t opcode     { static_cast<codage::opcode_t>(instruction & codage::masque::OPCODE) };

        codage::opcode::id_reg_t rd { static_cast<codage::opcode::id_reg_t>(instruction & codage::masque::RD) };
        codage::opcode::id_reg_t rt { static_cast<codage::opcode::id_reg_t>(instruction & codage::masque::RT) };
        codage::opcode::id_reg_t rs { static_cast<codage::opcode::id_reg_t>(instruction & codage::masque::RS) };

        if (opcode == codage::opcode::EXE)
        {
            exe();
            return;
        }

        int num_rd = rd >> 7;
        int num_rt = rt >> 4;
        int num_rs = rs >> 1;

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
                    add(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::DIF :
                    dif(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::MUL :
                    mul(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::DIV :
                    div(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::MOD :
                    mod(num_rd, num_rt, num_rs);
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
                    etl(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::OUL :
                    oul(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::XOR :
                    oux(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::NSI :
                    nsi(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::NSD :
                    nsd(num_rd, num_rt, num_rs);
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
                    cmm(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::COM :
                    com(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::RMM :
                    rmm(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::ROM :
                    rom(num_rd, num_rt, num_rs);
                    break;
            }
        }

        constexpr codage::opcode_t EST_BITS { 0x8000 };

        /*
         * opcode des operations arithmetiques commence par (100xxx)
         * 0xe000 permet de recuperer les trois premiers bits et les comparer avec 0x2000
         */

        if ((opcode & 0xe000) == EST_BITS)
        {
            switch(opcode)
            {
                case codage::opcode::CBG :
                    cbg(num_rd, 0xff);
                    break;
                case codage::opcode::DGL :
                    dgl(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::DDL :
                    ddl(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::CBD :
                    cbd(num_rd, 0xff);
                    break;
            }
        }

        constexpr codage::opcode_t EST_BRANCHEMENT { 0xa000 };

        /*
         * opcode des operations arithmetiques commence par (101xxx)
         * 0xe000 permet de recuperer les trois premiers bits et les comparer avec 0xa000
         */

        if ((opcode & 0xe000) == EST_BRANCHEMENT)
        {
            switch(opcode)
            {
                case codage::opcode::ALL :
                    all(num_rd, num_rt);
                    break;
                case codage::opcode::SEG :
                    seg(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::SIS :
                    sis(num_rd, num_rt, num_rs);
                    break;
                case codage::opcode::SIE :
                    sie(num_rd, num_rt, num_rs);
                    break;
                default :
                    break;
            }
        }
    }

    void Cpu::exec(const std::string &instruction)
    {
        codage::instruction_t instruction_executable = instruction_depuis_chaine(chaine_formelle(instruction));

        exec(instruction_executable);
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

    void Cpu::cbg(const int &rd, const mot_t &valeur)
    {
        mot_t bits { valeur };
        Cpu::m_registre[rd] = (Cpu::m_registre[rd] & 0x00ff ) | (bits << 2);
    }

    void Cpu::dgl(const int &rd, const int &rt, const int &rs)
    {
        Cpu::m_registre[rd] = Cpu::m_registre[rt] << (Cpu::m_registre[rs] & 0x000f);
    }

    void Cpu::ddl(const int &rd, const int &rt, const int &rs)
    {
        Cpu::m_registre[rd] = Cpu::m_registre[rt] >> (Cpu::m_registre[rs] & 0x000f);
    }

    void Cpu::cbd(const int &rd, const mot_t &valeur)
    {
        mot_t bits { valeur };
        Cpu::m_registre[rd] = (Cpu::m_registre[rd] & 0xff00 ) | bits;
    }

    void Cpu::all(const int &rd, const int &rt)
    {
        Cpu::m_compteur_oridnal = Cpu::m_registre[rd] + Cpu::m_registre[rt];
    }

    void Cpu::seg(const int &rd, const int &rt, const int &rs)
    {
        if (Cpu::m_registre[rt] == Cpu::m_registre[rs])
            Cpu::m_compteur_oridnal = Cpu::m_registre[rd];
    }

    void Cpu::sis(const int &rd, const int &rt, const int &rs)
    {
        if (Cpu::m_registre[rt] < Cpu::m_registre[rs])
            Cpu::m_compteur_oridnal = Cpu::m_registre[rd];

    }

    void Cpu::sie(const int &rd, const int &rt, const int &rs)
    {
        if (Cpu::m_registre[rt] <= Cpu::m_registre[rs])
            Cpu::m_compteur_oridnal = Cpu::m_registre[rd];
    }

    std::string Cpu::ecrire_chaine(const int &longueur)
    {
        std::string chaine = "\0";

        int i { 0 };
        for (i = 0; i < longueur; ++i)
            chaine += m_memoire[m_registre[5] + i];

        return chaine;
    }

    void Cpu::exe()
    {
        // depend selon la valeur du registre 07
        switch (m_registre[7])
        {
            case 0 :
                std::cout << "Programme execute avec succes.\n";
                break;
            case 1 :
                std::cout << Cpu::m_registre[6] << '\n';
                break;
            case 2 :
                Cpu::ecrire_chaine(Cpu::m_registre[6]);
                break;
            case 3 :
                std::cin >> Cpu::m_registre[6];
                break;
            default :
                std::cout << "Programme avorte.\n";
                exit(EXIT_FAILURE);
                break;
        }
    }
}
