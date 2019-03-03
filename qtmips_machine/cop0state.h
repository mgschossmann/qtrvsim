// SPDX-License-Identifier: GPL-2.0+
/*******************************************************************************
 * QtMips - MIPS 32-bit Architecture Subset Simulator
 *
 * Implemented to support following courses:
 *
 *   B35APO - Computer Architectures
 *   https://cw.fel.cvut.cz/wiki/courses/b35apo
 *
 *   B4M35PAP - Advanced Computer Architectures
 *   https://cw.fel.cvut.cz/wiki/courses/b4m35pap/start
 *
 * Copyright (c) 2017-2019 Karel Koci<cynerd@email.cz>
 * Copyright (c) 2019      Pavel Pisa <pisa@cmp.felk.cvut.cz>
 *
 * Faculty of Electrical Engineering (http://www.fel.cvut.cz)
 * Czech Technical University        (http://www.cvut.cz/)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 ******************************************************************************/

#ifndef COP0STATE_H
#define COP0STATE_H

#include <QObject>
#include <cstdint>
#include <machinedefs.h>
namespace machine {

class Core;

class Cop0State : public QObject {
    Q_OBJECT
    friend class Core;
public:
    enum Cop0Regsisters {
        Unsupported = 0,
        UserLocal,
        BadVAddr, // Reports the address for the most recent address-related exception
        Count,    // Processor cycle count
        Compare,  // Timer interrupt control
        Status,   // Processor status and control
        Cause,    // Cause of last exception
        EPC,      // Program counter at last exception
        EBase,    // Exception vector base register
        Config,   // Configuration registers
        COP0REGS_CNT,
    };

    Cop0State(Core *core = nullptr);
    Cop0State(const Cop0State&);

    std::uint32_t read_cop0reg(enum Cop0Regsisters reg) const;
    std::uint32_t read_cop0reg(std::uint8_t rd, std::uint8_t sel) const; // Read coprocessor 0 register
    void write_cop0reg(enum Cop0Regsisters reg, std::uint32_t value);
    void write_cop0reg(std::uint8_t reg, std::uint8_t sel, std::uint32_t value); // Write coprocessor 0 register

    bool operator ==(const Cop0State &c) const;
    bool operator !=(const Cop0State &c) const;

    void reset(); // Reset all values to zero

protected:
    void setup_core(Core *core);
    void update_execption_cause(enum ExceptionCause excause, bool in_delay_slot);

private:

    typedef std::uint32_t (Cop0State::*reg_read_t)
                  (enum Cop0Regsisters reg) const;

    typedef void (Cop0State::*reg_write_t)
                  (enum Cop0Regsisters reg, std::uint32_t value);

    struct cop0reg_desc_t {
        const char *name;
        std::uint32_t write_mask;
        reg_read_t reg_read;
        reg_write_t reg_write;
    };

    static const cop0reg_desc_t cop0reg_desc[COP0REGS_CNT];

    std::uint32_t read_cop0reg_default(enum Cop0Regsisters reg) const;
    void write_cop0reg_default(enum Cop0Regsisters reg, std::uint32_t value);
    Core *core;
    std::uint32_t cop0reg[COP0REGS_CNT]; // coprocessor 0 registers
};

}

Q_DECLARE_METATYPE(machine::Cop0State)

#endif // COP0STATE_H
