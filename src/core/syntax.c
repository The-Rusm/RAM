/* syntax.c the global syntax of RAM
    RAM the Rusm Assembly Machine.
    Copyright(C) 2026 Alexander Silaev and The Rusm Org.
    See the https://github.com/The-Rusm/RAM

    Licensed under GNU General Public License v3.
    SPDX-License-Identifier: GPL-3-or-later.
    RAM is Free Software.
*/

#include <syntax.h>
#include <tce.h>
#include <string.h>

static const char *op_strs[] = {
    #define X(name, str, args) str,
        ALL_INSTRUCTIONS
    #undef X
};

static const int op_args_count[] = {
#define X(name, str, args) args,
    ALL_INSTRUCTIONS
#undef X
};

_RAM_OpCode syntax_lookup(const char *json_op_str)
{
    if (!json_op_str) return OP_NOP;

    for (int i = 0; i < OP_MAX_COUNT; i++)
    {
        if (strcmp(json_op_str, op_strs[i]) == 0)
            return (_RAM_OpCode)i;
    }

    char errbuf[128];
    snprintf(errbuf, sizeof(errbuf), "RAM Syntax Error: Unknown operation '%s'", json_op_str);
    throw(errbuf);
    
    return OP_NOP;
}

const char *syntax_op_to_str(_RAM_OpCode op)
{
    if (op >= OP_MAX_COUNT) return "unknown";
    return op_strs[op];
}

int syntax_expected_args(_RAM_OpCode op)
{
    if (op >= OP_MAX_COUNT) return 0;
    return op_args_count[op];
}

void syntax_validate_instruction(const _RAMInstruction *instruction)
{
    if (instruction->op >= OP_MAX_COUNT) throw("RAM Syntax Error: Non-valid Operation cod!");

    int expected = syntax_expected_args(instruction->op);
    if (instruction->arg_count != expected) {
        char errbuf[256];
        snprintf(errbuf, sizeof(errbuf), "RAM Syntax Error: Command '%s' expected %d arguments, but received %d",
            syntax_op_to_str(instruction->op), expected, instruction->arg_count);
        throw(errbuf); 
    }
}