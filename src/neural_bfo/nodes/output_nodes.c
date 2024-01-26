//
// Created by kosenko on 26.01.24.
//

#include "output_nodes.h"

void* output_node_forward_prop(node *output_node, void *args){
    ((struct output_node_data*)output_node->internal_data)->data=args;
    return NULL;
}

node* create_output_node(){
    node *result= calloc(1, sizeof(node));
    result->forward_prop=output_node_forward_prop;
    return result;
}