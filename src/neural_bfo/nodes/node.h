//
// Created by kosenko on 20.01.24.
//

#ifndef BRAIN_CRAFT_SRC_NEURAL_BFO_NODE_H
#define BRAIN_CRAFT_SRC_NEURAL_BFO_NODE_H

typedef struct node{
    void* (*forward_prop)(struct node *, void *);
    void* internal_data;
}node;
typedef struct{
    node* forward_prop_node;
    void *(*back_prop)(node *, void *);
    void *back_prop_data;
}back_prop_node;
#endif //BRAIN_CRAFT_SRC_NEURAL_BFO_NODE_H
