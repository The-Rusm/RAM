#include "tce.h"

_Thread_local jmp_buf *g_exception_stack[64];
_Thread_local int g_stack_pointer = -1;
_Thread_local TCE_Ctx g_last_error;
void error_type_not_supported (void);

void g_throw_int (int value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_INT, {.i = value}
    };

    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (int: %d)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_long_int (long int value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_LONG_INT, {.li = value}
    };

    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (long int: %ld)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_long (long value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_LONG, {.l = value}
    };

    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (long: %ld)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_long_long (long long value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_LONG_LONG, {.ll = value}
    };

    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (int: %lld)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_string (const char *value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_STRING, {.string = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (string: %s)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_pointer (void *value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_POINTER, {.pointer = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (pointer: %p)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_float (float value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_FLOAT, {.f = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (float: %.3f)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_double (double value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_DOUBLE, {.d = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (double: %.3lf)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_uint8b (uint8_t value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_UINT8, {.ue = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (unsigned int 8 bit: %u)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_uint16b (uint16_t value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_UINT16, {.us = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (unsigned int 16 bit: %u)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_uint32b (uint32_t value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_UINT32, {.ut = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (unsigned int 32 bit: %u)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_uint64b (uint64_t value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_UINT64, {.usty = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (unsigned int 64 bit: %lu)\n", value);
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}

void g_throw_boolean (bool value)
{
    g_last_error = (TCE_Ctx)
    {
        TCE_BOOLEAN, {.b = value}
    };
    if (g_stack_pointer < 0)
    {
        fprintf (stderr, "Panic: Uncaught Exception (int: %s)\n", value ? "true" : "false");
        exit (EXIT_FAILURE);
    }
    jmp_buf *target = g_exception_stack[g_stack_pointer--];
    longjmp (*target, 1);
}