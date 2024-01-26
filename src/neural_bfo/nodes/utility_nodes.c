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

void* order_node_forward_prop(node* order_node, void* args){
    struct order_enforcement* result= calloc(1, sizeof(struct order_enforcement));
    result->data=args;
    struct order_node_data* data=order_node->internal_data;
    result->order_number=data->order_number;
    data->next->forward_prop(data->next, result);
    return NULL;
}
node* create_order_node(unsigned char order_number){
    node* result= calloc(1, sizeof(node));
    struct order_node_data* internal_data= calloc(1, sizeof(struct order_node_data));
    result->internal_data=internal_data;
    internal_data->order_number=order_number;
    result->forward_prop=order_node_forward_prop;
    return result;
}