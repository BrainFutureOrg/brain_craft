//
// Created by kosenko on 23.01.24.
//

#ifndef BRAIN_CRAFT_SRC_GARBAGE_COLLECTOR_BFO_SELF_FREE_H
#define BRAIN_CRAFT_SRC_GARBAGE_COLLECTOR_BFO_SELF_FREE_H

#include <stdlib.h>
//self-free which frees its ptr_to_free when it has no uses, maximum 2^8 uses possible. Note that it's freed when usage counter is 0 before free attempt
typedef struct{
    void *ptr_to_free;
    unsigned char usage_counter;
}self_free_simple_small;
//self-free which calls free_func on ptr_to_free, frees its ptr_to_free when it has no uses, maximum 2^8 uses possible. Note that it's freed when usage counter is 0 before free attempt
typedef struct{
    void *ptr_to_free;
    unsigned char usage_counter;
    void(*free_func)(void*);
}self_free_small;
//self-free which frees its ptr_to_free when it has no uses, maximum [unsigned int maximal value] uses possible. Note that it's freed when usage counter is 0 before free attempt
typedef struct{
    void *ptr_to_free;
    unsigned int usage_counter;
}self_free_simple;
//self-free which calls free_func on ptr_to_free, frees its ptr_to_free when it has no uses, maximum [unsigned int maximal value] uses possible. Note that it's freed when usage counter is 0 before free attempt
typedef struct{
    void *ptr_to_free;
    unsigned int usage_counter;
    void(*free_func)(void*);
}self_free;

void free_if_unused_simple_small(self_free_simple_small *to_free);

void increment_usage_simple_small(self_free_simple_small *to_free);

void free_if_unused_small(self_free_small *to_free);

void increment_usage_small(self_free_small *to_free);

void free_if_unused_simple(self_free_simple *to_free);

void increment_usage_simple(self_free_simple *to_free);

void free_if_unused(self_free *to_free);

void increment_usage(self_free *to_free);

self_free_simple_small *create_self_free_simple_small(void *ptr_to_free);

self_free_simple *create_self_free_simple(void *ptr_to_free);

self_free_small *create_self_free_small(void *ptr_to_free, void(*free_func)(void*));

self_free *create_self_free(void *ptr_to_free, void(*free_func)(void*));

#endif //BRAIN_CRAFT_SRC_GARBAGE_COLLECTOR_BFO_SELF_FREE_H
