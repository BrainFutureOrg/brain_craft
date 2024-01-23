//
// Created by kosenko on 23.01.24.
//

#include "self_free.h"

void free_if_unused_simple_small(self_free_simple_small *to_free){
    if(to_free->usage_counter){
        to_free->usage_counter--;
    }else{
        free(to_free->ptr_to_free);
        free(to_free);
    }
}

void increment_usage_simple_small(self_free_simple_small *to_free){
    to_free->usage_counter++;
}

void free_if_unused_simple(self_free_simple *to_free){
    if(to_free->usage_counter){
        to_free->usage_counter--;
    }else{
        free(to_free->ptr_to_free);
        free(to_free);
    }
}

void increment_usage_simple(self_free_simple *to_free){
    to_free->usage_counter++;
}

void free_if_unused_small(self_free_small *to_free){
    if(to_free->usage_counter){
        to_free->usage_counter--;
    }else{
        to_free->free_func(to_free->ptr_to_free);
        free(to_free->ptr_to_free);
        free(to_free);
    }
}

void increment_usage_small(self_free_small *to_free){
    to_free->usage_counter++;
}

void free_if_unused(self_free *to_free){
    if(to_free->usage_counter){
        to_free->usage_counter--;
    }else{
        to_free->free_func(to_free->ptr_to_free);
        free(to_free->ptr_to_free);
        free(to_free);
    }
}

void increment_usage(self_free *to_free){
    to_free->usage_counter++;
}

self_free_simple_small *create_self_free_simple_small(void *ptr_to_free){
    self_free_simple_small *result= calloc(1, sizeof(self_free_simple_small));
    result->ptr_to_free=ptr_to_free;
    result->usage_counter=0;
    return result;
}

self_free_simple *create_self_free_simple(void *ptr_to_free){
    self_free_simple *result= calloc(1, sizeof(self_free_simple));
    result->ptr_to_free=ptr_to_free;
    result->usage_counter=0;
    return result;
}

self_free_small *create_self_free_small(void *ptr_to_free, void(*free_func)(void*)){
    self_free_small *result= calloc(1, sizeof(self_free_small));
    result->ptr_to_free=ptr_to_free;
    result->usage_counter=0;
    result->free_func=free_func;
    return result;
}

self_free *create_self_free(void *ptr_to_free, void(*free_func)(void*)){
    self_free *result= calloc(1, sizeof(self_free));
    result->ptr_to_free=ptr_to_free;
    result->usage_counter=0;
    result->free_func=free_func;
    return result;
}