//
// Created by kosenko on 22.01.24.
//

#include "fully_connected_nodes.h"
void *fully_connected_forward_prop(node *node, void *args){
    struct fully_connected_internal_data *data=node->internal_data;
    matrix *Wx= calloc(1, sizeof(matrix));
    *Wx= matrix_multiplication(*data->weights, *((matrix*)((self_free_simple_small*)args)->ptr_to_free));
    free_if_unused_simple_small_matrix(args);
    matrix_addition_inplace(*Wx, *data->bias);
    if(data->back_node!=NULL){
        matrix *Wx_copy= calloc(1, sizeof(matrix));
        *Wx_copy= matrix_copy(*Wx);
        ((struct fully_connected_back_internal_data*)data->back_node->back_prop_data)->unactivated_output=Wx_copy;
    }
    matrix_function_to_elements(*Wx, data->activation_elementwise);
    self_free_simple_small *self_free_out= create_self_free_simple_small(Wx);
    if(data->back_node!=NULL){
        self_free_out->usage_counter++;
        ((struct fully_connected_back_internal_data*)data->back_node->back_prop_data)->activated_output=self_free_out;
    }
    data->next->forward_prop(data->next, self_free_out);
    return NULL;
}

node *create_fully_connected_node(){
    node *result= calloc(1, sizeof(node));
    struct fully_connected_internal_data* data= calloc(1, sizeof(struct fully_connected_internal_data));
    result->internal_data=data;
    data->back_node=NULL;
    result->forward_prop=fully_connected_forward_prop;
    return result;
}