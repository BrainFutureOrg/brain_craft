//
// Created by kosenko on 20.01.24.
//

#ifndef BRAIN_CRAFT_SRC_NEURAL_BFO_UTILITY_NODES_H
#define BRAIN_CRAFT_SRC_NEURAL_BFO_UTILITY_NODES_H
#include "../../libraries/arrays_bfo/Array_type.h"
#include "node.h"
#include "stdlib.h"

//internal data for branch_node
struct branch_internal_data{
    array_voidp *node_arr;
};

//creates branch_node with pointer to array of children nodes
//branch_node when called calls each of its children with same arguments that were used for it's call
node* create_branch_node(array_voidp* node_arr);

//frees branch_node (but not children array)
void free_branch_node(node* branch_node);

#endif //BRAIN_CRAFT_SRC_NEURAL_BFO_UTILITY_NODES_H
