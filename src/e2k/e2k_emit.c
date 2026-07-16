/* e2k_emit.c the e2k specs and cmds for RAM
    RAM the Rusm Assembly Machine.
    Copyright(C) 2026 Alexander Silaev and The Rusm Org.
    See the https://github.com/The-Rusm/RAM

    Licensed under GNU General Public License v3.
    SPDX-License-Identifier: GPL-3-or-later.
    RAM is Free Software.

    E2K is a MCST's arch.
*/
#include <e2k.h>
#include <syntax.h>
#include <tce.h>
#include <stdio.h>

static uint8_t e2k_encode_opcode(_RAM_OpCode op)
{
    switch(op)
    {
        case OP_NOP:  return 0x00;
        case OP_ADDS: return 0x10;
        case OP_FADDS:return 0x50;
        case OP_ADDD: return 0x11;
        case OP_FADDD:return 0x51;
        case OP_SUBS: return 0x12;
        case OP_SUBD: return 0x13;
        case OP_MULS: return 0x14;
        case OP_MULD: return 0x15;
        case OP_DIVS: return 0x16;
        case OP_DIVD: return 0x17;
        case OP_SDIVS:return 0x18;
        case OP_ANDS: return 0x40;
        case OP_ANDD: return 0x41;
        case OP_ORS:  return 0x42;
        case OP_ORD:  return 0x43;
        case OP_XORS: return 0x44;
        case OP_XORD: return 0x45;
        case OP_NOTS: return 0x46;
        case OP_NOTD: return 0x47;
        case OP_LD:   return 0x20;
        case OP_ST:   return 0x21;
        case OP_JMP:  return 0x30;
        case OP_CT:   return 0x31;
        case OP_RTN:  return 0x32;
        default:      return 0xFF;
    }
}

void e2k_emit_bundle(const _ETK_VliwBundle *bundle, FILE *out_file)
{
    if (!out_file) throw("RAM E2K Error: Output file not opened or NULL");

    uint32_t bundle_header = 0;
    for (int ch = 0; ch < 6; ch++)
        if (bundle->slots[ch].is_active) bundle_header |= (1 << ch);
    
    if (fwrite(&bundle_header, sizeof(uint32_t), 1, out_file) != 1)
        throw("RAM E2K Emit Error: Cannot write packet header into file");

    for (int ch = 0; ch < 6; ch++)
    {
        uint32_t slot_bytes = 0;
        _ETK_Slot slot = bundle->slots[ch];

        if (slot.is_active)
        {
            uint8_t bin_op = e2k_encode_opcode(slot.op);
            
            // Магические побитовые сдвиги Си (Битовая упаковка Intel-подобной команды):
            // [ 8 бит: Opcode ] [ 6 бит: Dest Reg ] [ 6 бит: Src1 Reg ] [ 6 бит: Src2 Reg ] [ 6 бит: Резерв ]
            slot_bytes |= (uint32_t)bin_op;
            slot_bytes |= ((uint32_t)(slot.reg_dest & 0x3F)) << 8;
            slot_bytes |= ((uint32_t)(slot.reg_srcO & 0x3F)) << 14;
            slot_bytes |= ((uint32_t)(slot.reg_srcT & 0x3F)) << 20;
        } else
        {
            slot_bytes = 0x00000000;
        }

        if (fwrite(&slot_bytes, sizeof(uint32_t), 1, out_file) != 1)
            throw("RAM E2K Emit Error: Failed to write slot data in file.");
    }
}