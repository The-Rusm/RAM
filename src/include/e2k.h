/* e2k.h the e2k specs and cmds for RAM
    RAM the Rusm Assembly Machine.
    Copyright(C) 2026 Alexander Silaev and The Rusm Org.
    See the https://github.com/The-Rusm/RAM

    Licensed under GNU General Public License v3.
    SPDX-License-Identifier: GPL-3-or-later.
    RAM is Free Software.

    E2K is a MCST's arch.
*/
#ifndef __RAM_E2K__
#define __RAM_E2K__

#include <syntax.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    _RAM_OpCode op;
    uint8_t reg_dest; 
    uint8_t reg_srcO; // count is the first word of number there is O - One.
    uint8_t reg_srcT;
    bool is_active;
} _ETK_Slot;

typedef struct {
    _ETK_Slot slots[6];
} _ETK_VliwBundle;

uint8_t e2k_get_supported_channels(_RAM_OpCode op);
int e2k_schedule_bundle(const _RAMInstruction *raw_insts, int inst_count, _ETK_VliwBundle *out_bundle);
uint8_t e2k_get_supported_channels(_RAM_OpCode op);
void e2k_emit_bundle(const _ETK_VliwBundle *bundle, FILE *out_file);

#endif /*__RAM_E2K__*/