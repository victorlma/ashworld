#ifndef MDEFS_H_
#define MDEFS_H_

//Handy types

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int  u32;
typedef unsigned long int u64;

typedef signed char s8;
typedef signed short int s16;
typedef signed int  s32;
typedef signed long int s64;

typedef float f32;
typedef double f64;

typedef enum {FALSE, TRUE} b32;

#define KB(x)  x * 1024
#define MB(x)  KB(x) * 1024
#define GB(x)  MB(x) * 1024

typedef struct
{
    const char *file;
    const char *func_name;
    u64         line;

} ASSERT_METADATA;

#define ASSERT_POP_METADATA (ASSERT_METADATA) {__FILE__, __func__, __LINE__}

#if DEBUG 
#include "stdio.h"
#include "stdlib.h"

void m_assert(b32 expr, ASSERT_METADATA  *meta, const char *exprstr)
{
    if (!expr) {
        printf("Assert with expr: %s  Failed at: \n File: %s\n Function: %s at line %ld\n",
               exprstr, meta->file, meta->func_name, meta->line);
        exit(1);
    }
}

#define DEBUG_ASSERT(expr) \
        m_assert(expr, (ASSERT_METADATA []) {ASSERT_POP_METADATA}, #expr)
#else

#define DEBUG_ASSERT(expr)
#endif

#endif
