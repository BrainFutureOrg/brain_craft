//
// Created by kosenko on 23.01.24.
//

#ifndef BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_COMMUNICATION_STRUCTURES_H
#define BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_COMMUNICATION_STRUCTURES_H

#include "../../garbage_collector_bfo/self_free.h"
#include "../../libraries/math_bfo/matrix_operations.h"

//frees self_free_simple_small with matrix * as ptr_to_free
void free_if_unused_simple_small_matrix(self_free_simple_small* to_free);

#endif //BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_COMMUNICATION_STRUCTURES_H
