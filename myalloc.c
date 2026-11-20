#include "myalloc.h"
#include <unistd.h>    // sbrk()
#include <string.h>    // memset(), memcpy()
#include <stdio.h>     // fprintf()
#include <stdint.h>    // uintptr_t

#define ALIGN16(x)   (((size_t)(x) + 15) & ~(size_t)15)


typedef struct block {
    size_t       size;
    int          free;
    struct block *next;
} block_t;


#define HEADER_SIZE  ALIGN16(sizeof(block_t))


static block_t    *head   = NULL;      
static fit_policy_t policy = FIT_FIRST;  
