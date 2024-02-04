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
#include "../../libraries/arrays_bfo/Array_type.h"
#include "../optimizers/optimizer.h"

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
    self_free_simple_small *prev_activated_output;
    matrix *unactivated_output;
    node *forward_node;
    double (*activation_derivative)(double);
    array_voidp *regularizers;
    optimizer *inplace_optimizer;
    back_prop_node* next;
    char flags;//1 - freeze
};

//creates empty fully_connected_node
node *create_fully_connected_node();

//creates fully_connected_back_prop_node with only necessary data and links it to forward_node
back_prop_node *create_fully_connected_back_prop_node(node* forward_node, double (*activation_derivative)(double));

//corresponding forward nodes back node pointer becomes NULL. Note that back nodes pointer to forward pointer remains (unlinked node will be freed or reconnected explicitly  either way as link is necessary)
void unlink_fully_connected_back_forward_nodes(back_prop_node* back_node);

#endif //BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_FULLY_CONNECTED_NODES_H
