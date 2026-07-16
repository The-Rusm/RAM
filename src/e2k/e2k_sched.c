/* e2k_sched.c the e2k scheduler for RAM
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
#include <string.h>
#include <stdint.h>

uint8_t e2k_get_supported_channels(_RAM_OpCode op)
{
    switch (op) {
        case OP_ADDS:
        case OP_ADDD:
        case OP_SUBS:
        case OP_SUBD:
        case OP_ANDS:
        case OP_ANDD:
        case OP_ORS:
        case OP_ORD:
        case OP_XORS:
        case OP_XORD:
        case OP_NOTS:
        case OP_NOTD:
        case OP_MOV:
            return 0x0F; /* 001111 ch 0, 1, 2, 3 */


        case OP_MULS:
        case OP_MULD:
        case OP_FADDS:
        case OP_FADDD:
            return 0x03; /* 000011 ch 0, 1*/
        
        
        case OP_DIVS:
        case OP_DIVD:
        case OP_SDIVS:
            return 0x01; /* 000001 ch 0 only */
        
        
        case OP_LD:
        case OP_ST:
        case OP_JMP:
        case OP_CT: 
        case OP_RTN: 
            return 0x30; /* 110000 ch 4, 5 - memory*/
        
        case OP_NOP:
        default:
            return 0x00; /* command does not use standard ALU channels. */
    }
}

int e2k_schedule_bundle(const _RAMInstruction *raw_insts, int inst_count, _ETK_VliwBundle *out_bundle)
{
    memset(out_bundle, 0, sizeof(_ETK_VliwBundle));

    uint8_t busy_ch = 0x00;
    int packed_count = 0;

    for (int i = 0; i < inst_count; i++)
    {
        _RAMInstruction inst = raw_insts[i];

        uint32_t curr_dst = (inst.arg_count > 0 && inst.args[0].type == ARG_REG) ? inst.args[0].val : 0xFFFFFFFF;
        uint32_t curr_srcO = (inst.arg_count > 1 && inst.args[1].type == ARG_REG) ? inst.args[1].val : 0xFFFFFFFF;
        uint32_t curr_srcT = (inst.arg_count > 2 && inst.args[2].type == ARG_REG) ? inst.args[2].val : 0xFFFFFFFF;
        
        bool is_hazard = false;

        for (int prev = 0; prev < i; prev++)
        {
            _RAMInstruction p_inst = raw_insts[prev];
            uint32_t prev_dst = (p_inst.arg_count > 0 && p_inst.args[0].type == ARG_REG) ? p_inst.args[0].val : 0xFFFFFFFF;

            if (prev_dst != 0xFFFFFFFF)
            {
                /*WAW conflict: 2 instructions write to the same register in the same tact*/
                if (curr_dst == prev_dst) { is_hazard = true; break; }

                /*RAW conflict: current instruction read same that a previous will write only at the end of tact*/
                if (curr_srcO == prev_dst || curr_srcT == prev_dst) { is_hazard = true; break; }
            }
        }

        if (is_hazard) break;

        uint8_t allowed_mask = e2k_get_supported_channels(inst.op);
        bool scheduled = false;

        for (int ch = 0; ch < 6; ch++)
        {
            if ((allowed_mask & (1 << ch)) && !(busy_ch & (1 << ch)))
            {
                busy_ch |= (1 << ch);

                out_bundle->slots[ch].op = inst.op;
                out_bundle->slots[ch].reg_dest = (curr_dst != 0xFFFFFFFF) ? curr_dst : 0;
                out_bundle->slots[ch].reg_srcO = (curr_srcO != 0xFFFFFFFF) ? curr_srcO : 0;
                out_bundle->slots[ch].reg_srcT = (curr_srcT != 0xFFFFFFFF) ? curr_srcT : 0;
                out_bundle->slots[ch].is_active = true;

                scheduled = true;
                packed_count++;
                break;
            }
        }

        if (!scheduled)
        {
            char errmsg[128];
            snprintf(errmsg, sizeof(errmsg), "RAM E2K Scheduler Error: No free ALU channels found for instruction '%s' in this tact", syntax_op_to_str(inst.op));
            throw(errmsg);
        }
    }

    return packed_count;
}