//
// Created by kosenko on 23.01.24.
//

#include "communication_structures.h"

void free_if_unused_simple_small_matrix(self_free_simple_small* to_free){
    if(to_free->usage_counter){
        to_free->usage_counter--;
    }else{
        matrix_free(*((matrix*)to_free->ptr_to_free));
        free(to_free->ptr_to_free);
        free(to_free);
    }
}