#ifndef TRY_CATCH_EXCEPTION_H
#define TRY_CATCH_EXCEPTION_H

#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <imm/IntMemoryManager.h>

typedef struct {
    enum {
        TCE_INT, 
        TCE_STRING,
        TCE_FLOAT,
        TCE_DOUBLE,
        TCE_POINTER,
        TCE_UINT8,
        TCE_UINT16,
        TCE_UINT32,
        TCE_UINT64,
        TCE_BOOLEAN,
        TCE_LONG,
        TCE_LONG_LONG,
        TCE_LONG_INT        
    } type;

    union {
        const char *string;
        int i;
        void *pointer;
        float f;
        bool b;
        double d;
        uint8_t ue;
        long l;
        long long ll;
        long int li;
        uint16_t us;
        uint32_t ut;
        uint64_t usty;
    } data;
} TCE_Ctx;

extern _Thread_local jmp_buf *g_exception_stack[64];
extern _Thread_local int g_stack_pointer;
extern _Thread_local TCE_Ctx g_last_error;
extern void error_type_not_supported (void);

void g_throw_uint8b (uint8_t value);
void g_throw_uint16b (uint16_t value);
void g_throw_uint32b (uint32_t value);
void g_throw_uint64b (uint64_t value);
void g_throw_int (int value);
void g_throw_long (long value);
void g_throw_long_long (long long value);
void g_throw_long_int (long int value);
void g_throw_string (const char *value);
void g_throw_pointer (void *value);
void g_throw_float (float value);
void g_throw_double (double value);
void g_throw_boolean (bool value);


#define throw(X) _Generic ((X), \
    int: g_throw_int, \
    char: g_throw_int, \
    signed char: g_throw_int, \
    float: g_throw_float, \
    double: g_throw_double, \
    const char *: g_throw_string, \
    char *: g_throw_string, \
    uint8_t: g_throw_uint8b, \
    uint16_t: g_throw_uint16b, \
    uint32_t: g_throw_uint32b, \
    uint64_t: g_throw_uint64b, \
    bool: g_throw_boolean, \
    void *: g_throw_pointer, \
    default: error_type_not_supported \
)(X) 

#define try \
    jmp_buf _env; \
    int _exception_thrown = 0; \
    g_exception_stack[++g_stack_pointer] = &_env; \
    if ((_exception_thrown = setjmp (_env)) == 0) \
        for (int _once = 1; _once; (_once = 0, g_stack_pointer--))


#define catch(type, var) \
    else if (_exception_thrown != 0) \
        for (type var = _TCE_Extract_##type(), \
            _once = (_exception_thrown = 0, 1); _once; (_once = 0))

#define finally \
    for (int _once = 1; _once; (_once = 0, _TCE_Finalize (&_exception_thrown)))

static inline int _TCE_Extract_int() { return g_last_error.data.i; }
static inline const char * _TCE_Extract_string() { return g_last_error.data.string; }
static inline void * _TCE_Extract_pointer() { return g_last_error.data.pointer; }
static inline void _TCE_Finalize (int *thrown)
{
    if (*thrown != 0)
    {
        if (g_stack_pointer >= 0)
        {
            longjmp (*g_exception_stack[g_stack_pointer--], 1);
        }
        else
        {
            fprintf (stderr, "Panic: Unhandled exception in finally\n");
            exit (EXIT_FAILURE);
        }
    }
}
static inline float _TCE_Extract_float() { return g_last_error.data.f; }
static inline double _TCE_Extract_double() { return g_last_error.data.d; }
static inline uint8_t _TCE_Extract_uint8_t() { return g_last_error.data.ue; }
static inline uint16_t _TCE_Extract_uint16_t() { return g_last_error.data.us; }
static inline uint32_t _TCE_Extract_uint32_t() { return g_last_error.data.ut; }
static inline uint64_t _TCE_Extract_uint64_t() { return g_last_error.data.usty; }
static inline bool _TCE_Extract_boolean() { return g_last_error.data.b; }

#endif /* TRY_CATCH_EXCEPTION_H */