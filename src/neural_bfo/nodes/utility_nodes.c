//
// Created by kosenko on 20.01.24.
//

#include "utility_nodes.h"

//forward pass for branch_node: calls children of branch_node with same arguments that were used for its call
void* branch_forward_pass(node* branch_node, void* args){
    struct branch_internal_data* data=branch_node->internal_data;
    for(uint i=0; i<data->node_arr->size; i++){
        ((void *(*)(void *))array_voidp_get_element(data->node_arr, i))(args);
    }
    return NULL;
}

node* create_branch_node(array_voidp* node_arr){
    node *result= calloc(1, sizeof(node));
    struct branch_internal_data* data= calloc(1, sizeof(struct branch_internal_data));
    data->node_arr=node_arr;
    result->internal_data=data;
    result->forward_prop=branch_forward_pass;
    return result;
}

void free_branch_node(node* branch_node){
    free(branch_node->internal_data);
    free(branch_node);
}