//
// Created by kosenko on 26.01.24.
//

#ifndef BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_OUTPUT_NODES_H
#define BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_OUTPUT_NODES_H

#include "../../garbage_collector_bfo/self_free.h"
#include "node.h"

struct output_node_data{
    void* data;
};

//creates output node
node* create_output_node();

#endif //BRAIN_CRAFT_SRC_NEURAL_BFO_NODES_OUTPUT_NODES_H
