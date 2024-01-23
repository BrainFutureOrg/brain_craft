//
// Created by kosenko on 22.01.24.
//

#ifndef BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_FULLY_CONNECTED_NODES_H
#define BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_FULLY_CONNECTED_NODES_H

#include "node.h"
#include "stdlib.h"
#include "../../libraries/math_bfo/matrix_operations.h"
#include "../../garbage_collector_bfo/self_free.h"
#include "../../libraries/math_bfo/matrix_operations.h"
#include "communication_structures.h"

//structure for internal data of fully connected node
struct fully_connected_internal_data{
    matrix *weights;
    matrix *bias;
    double (*activation_elementwise)(double);
    node* next;
    back_prop_node *back_node;//NULL if not training
};

//structure for backpropagation node for fully connected node
struct fully_connected_back_internal_data{
    self_free_simple_small *activated_output;
    matrix *unactivated_output;
    node *forward_node;
};

//creates empty fully_connected_node
node *create_fully_connected_node();
#endif //BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_FULLY_CONNECTED_NODES_H
