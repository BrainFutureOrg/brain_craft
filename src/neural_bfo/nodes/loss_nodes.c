//
// Created by kosenko on 26.01.24.
//

#include "loss_nodes.h"

void* loss_forward_prop(node* loss_node, void *args){
    struct loss_node_data* data=loss_node->internal_data;
    struct order_enforcement *arg=args;
    if(arg->order_number){
        data->labels_self_free=arg->data;
    }else{
        data->predictions_self_free=arg->data;
    }
    free(arg);
    if(data->predictions_self_free!=NULL&&data->labels_self_free!=NULL){
        if(data->back_node!=NULL&&data->flags&1){
            matrix* loss_d=data->loss_derivative(data->predictions_self_free->ptr_to_free, data->labels_self_free->ptr_to_free);
            if(data->next==NULL){
                free_if_unused_simple_small_matrix(data->predictions_self_free);
                free_if_unused_simple_small_matrix(data->labels_self_free);
            }
            data->back_node->back_prop(data->back_node, create_self_free_simple_small(loss_d));
        }
        if(data->next!=NULL){
            double* loss= calloc(1, sizeof(double));
            *loss=data->loss_func(data->predictions_self_free->ptr_to_free, data->labels_self_free->ptr_to_free);
            free_if_unused_simple_small_matrix(data->predictions_self_free);
            free_if_unused_simple_small_matrix(data->labels_self_free);
            data->next->forward_prop(data->next, loss);
        }
        data->predictions_self_free=NULL;
        data->labels_self_free=NULL;
    }
    return NULL;
}

node* create_loss_node(double(*loss_func)(matrix*,matrix*), matrix*(*loss_derivative)(matrix*,matrix*)){
    node *result= calloc(1, sizeof(node));
    struct loss_node_data* data= calloc(1, sizeof(struct loss_node_data));
    data->loss_func=loss_func;
    data->loss_derivative=loss_derivative;
    result->internal_data=data;
    result->forward_prop=loss_forward_prop;
    return result;
}