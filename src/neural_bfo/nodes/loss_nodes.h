//
// Created by kosenko on 26.01.24.
//

#ifndef BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_LOSS_NODES_H
#define BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_LOSS_NODES_H

#include "../../libraries/math_bfo/matrix_operations.h"
#include "node.h"
#include "../../garbage_collector_bfo/self_free.h"
#include "communication_structures.h"

#define LOSS_NODE_FLAG_TRAIN 1
struct loss_node_data{
    double (*loss_func)(matrix* predictions, matrix* labels);
    matrix* (*loss_derivative)(matrix* predictions, matrix* labels);
    back_prop_node *back_node;
    node *next;
    self_free_simple_small *labels_self_free;
    self_free_simple_small *predictions_self_free;
    char flags;//1 = training
};

//creates loss node using loss function and it's derivative. Note that flags and connections to other nodes are not set
node* create_loss_node(double(*loss_func)(matrix*,matrix*), matrix*(*loss_derivative)(matrix*,matrix*));

#endif //BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_LOSS_NODES_H
