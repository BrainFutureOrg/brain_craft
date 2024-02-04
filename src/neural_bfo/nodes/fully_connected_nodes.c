//
// Created by kosenko on 22.01.24.
//

#include "fully_connected_nodes.h"
void *fully_connected_forward_prop(node *n, void *args){
    struct fully_connected_internal_data *data=n->internal_data;
    matrix *Wx= calloc(1, sizeof(matrix));
    *Wx= matrix_multiplication(*data->weights, *((matrix*)((self_free_simple_small*)args)->ptr_to_free));
    if(data->back_node!=NULL){
        ((struct fully_connected_back_internal_data*)data->back_node->back_prop_data)->prev_activated_output=args;
        ((self_free_simple_small*)args)->usage_counter++;
    }
    free_if_unused_simple_small_matrix(args);
    matrix_addition_inplace(*Wx, *data->bias);
    if(data->back_node!=NULL){
        matrix *Wx_copy= calloc(1, sizeof(matrix));
        *Wx_copy= matrix_copy(*Wx);
        ((struct fully_connected_back_internal_data*)data->back_node->back_prop_data)->unactivated_output=Wx_copy;
    }
    matrix_function_to_elements(*Wx, data->activation_elementwise);
    self_free_simple_small *self_free_out= create_self_free_simple_small(Wx);
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

void* fully_connected_back_prop(back_prop_node *n, void *args){
    struct fully_connected_back_internal_data *data=n->back_prop_data;
    self_free_simple_small *prev_output=args;
    struct fully_connected_internal_data* data_forward=data->forward_node->internal_data;
    matrix_function_to_elements(*data->unactivated_output, data->activation_derivative);
    matrix_multiplication_elements_inplace(*data->unactivated_output, *((matrix*)prev_output->ptr_to_free));
    //now data->unactivated_output is bias gradient (without regularization)
    free_if_unused_simple_small_matrix(prev_output);
    data->inplace_optimizer->optimization_step(data->inplace_optimizer->internal_data, ((struct fully_connected_internal_data*)data->forward_node->internal_data)->bias, data->unactivated_output);
    matrix weight_gradient;//=//TODO: implement multiply with transposition and do it on prev_output and data->prev_activated_output
    if(data->regularizers!=NULL){
        //TODO: implement regularization
    }
    data->inplace_optimizer->optimization_step(data->inplace_optimizer->internal_data, ((struct fully_connected_internal_data*)data->forward_node->internal_data)->weights, &weight_gradient);
    matrix_free(weight_gradient);
    if(data->next!=NULL){
        self_free_simple_small *next_args= create_self_free_simple_small(calloc(1, sizeof(matrix)));
        //TODO: implement multiply with transposition and do it on weights and ... to form matrix for nex_args
        data->next->back_prop(data->next, next_args);
    }
    matrix_free(*data->unactivated_output);
    free(data->unactivated_output);
    return NULL;
}

back_prop_node *create_fully_connected_back_prop_node(node* forward_node, double (*activation_derivative)(double)){
    back_prop_node *result= calloc(1, sizeof(back_prop_node));
    ((struct fully_connected_internal_data*)forward_node->internal_data)->back_node=result;
    struct fully_connected_back_internal_data*data=result->back_prop_data;
    data->forward_node=forward_node;
    data->flags=0;
    data->activation_derivative=activation_derivative;
    return result;
}

void unlink_fully_connected_back_forward_nodes(back_prop_node* back_node){
    ((struct fully_connected_internal_data*)((struct fully_connected_back_internal_data*)back_node->back_prop_data)->forward_node->internal_data)->back_node=NULL;
}