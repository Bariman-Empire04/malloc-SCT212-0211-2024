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

void *my_malloc(size_t size) {
    
    if (size == 0) return NULL;

    
    size = ALIGN16(size);

    
    block_t *found = NULL;
    block_t *b     = head;

    while (b != NULL) {
        if (b->free && b->size >= size) {
            if (policy == FIT_FIRST) {
                found = b;
                break;  
            }
            
            if (found == NULL || b->size < found->size)
                found = b;
        }
        b = b->next;
    }

    
    if (found != NULL) {

        if (found->size >= size + HEADER_SIZE + 16) {

            block_t *newb = (block_t *)((char *)found + HEADER_SIZE + size);
            newb->size    = found->size - size - HEADER_SIZE;
            newb->free    = 1;
            newb->next    = found->next;

            found->next   = newb;
            found->size   = size;
        }
        found->free = 0;

        return (char *)found + HEADER_SIZE;
    }


    block_t *newb = sbrk(HEADER_SIZE + size);
    if (newb == (void *)-1) return NULL;

    newb->size = size;
    newb->free = 0;
    newb->next = NULL;

    
    if (head == NULL) {
        head = newb;
    } else {
        block_t *cur = head;
        while (cur->next != NULL) cur = cur->next;
        cur->next = newb;
    }
void my_free(void *ptr) {
    
    if (ptr == NULL) return;

    
    block_t *b = (block_t *)((char *)ptr - HEADER_SIZE);

    
    block_t *cur = head;
    while (cur != NULL && cur != b) cur = cur->next;
    if (cur == NULL) {
        fprintf(stderr, "my_free: invalid pointer %p\n", ptr);
        return;
    }

    
    if (b->free) {
        fprintf(stderr, "my_free: double free of %p\n", ptr);
        return;
    }

    b->free = 1;

 
    if (b->next != NULL && b->next->free) {
 
        b->size += HEADER_SIZE + b->next->size;
        b->next  = b->next->next;
    }

 
    cur = head;
    while (cur != NULL && cur->next != b) cur = cur->next;
    if (cur != NULL && cur->free) {
        cur->size += HEADER_SIZE + b->size;
        cur->next  = b->next;
    }
}

    return (char *)newb + HEADER_SIZE;
}
