/* syntax.h the global syntax of RAM
    RAM the Rusm Assembly Machine.
    Copyright(C) 2026 Alexander Silaev and The Rusm Org.
    See the https://github.com/The-Rusm/RAM

    Licensed under GNU General Public License v3.
    SPDX-License-Identifier: GPL-3-or-later.
    RAM is Free Software.
*/
#ifndef __RAM_syntax__
#define __RAM_syntax__

#include <stdint.h>
#include <stdbool.h>

/* X(name, json id, count of arguments) */
#define ALL_INSTRUCTIONS \
    X(OP_NOP,  "nop",   0) \
    X(OP_MOV,  "mov",   2) \
    X(OP_ADDS, "adds",  3) \
    X(OP_FADDS,"fadds", 3) \
    X(OP_SUBS, "subs",  3) \
    X(OP_MULS, "muls",  3) \
    X(OP_DIVS, "divs",  3) \
    X(OP_SDIVS,"sdivs", 3) \
    X(OP_ADDD, "addd",  3) \
    X(OP_FADDD,"faddd", 3) \
    X(OP_SUBD, "subd",  3) \
    X(OP_MULD, "muld",  3) \
    X(OP_DIVD, "divd",  3) \
    X(OP_LD,   "ld",    2) \
    X(OP_CT,   "ct",    3) \
    X(OP_RTN,  "rtn",   3) \
    X(OP_ST,   "st",    2) \
    X(OP_JMP,  "jmp",   1) \
    X(OP_ANDS, "ands",  3) /* S on the end - SHORT, D - DOUBLE */ \
    X(OP_ORS,  "ors",   3) \
    X(OP_XORS, "xors",  3) \
    X(OP_NOTS, "nots",  2) \
    X(OP_ANDD, "and",   3) \
    X(OP_ORD,  "or",    3) \
    X(OP_XORD, "xor",   3) \
    X(OP_NOTD, "not",   3)  

typedef enum {
#define X(name, str, args) name,
    ALL_INSTRUCTIONS
#undef X
    OP_MAX_COUNT
} _RAM_OpCode;

typedef enum {
    ARG_NONE = 0,
    ARG_REG, // Register
    ARG_IMM, // Immediate
    ARG_MEM // pointer
} _RAM_ArgType;

typedef struct {
    _RAM_ArgType type;
    uint32_t val;
} _RAMArg;

typedef struct {
    _RAM_OpCode op;
    _RAMArg args[4];
    int arg_count;
    bool is_vliw_packet;
} _RAMInstruction;

_RAM_OpCode syntax_lookup(const char *json_op_str);
const char *syntax_op_to_str(_RAM_OpCode op);
int syntax_expected_args(_RAM_OpCode op);
void syntax_validate_instruction(const _RAMInstruction *instruction);

#endif /*__RAM_syntax__*/