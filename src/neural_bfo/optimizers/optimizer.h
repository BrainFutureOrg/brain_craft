//
// Created by kosenko on 24.01.24.
//

#ifndef BRAIN_CRAFT_SRC_NEURAL_BFO_OPTIMIZERS_OPTIMIZER_H
#define BRAIN_CRAFT_SRC_NEURAL_BFO_OPTIMIZERS_OPTIMIZER_H

#include "../../libraries/math_bfo/matrix_operations.h"

typedef struct{
    void (*optimization_step)(void* internal_data, matrix* to_change, matrix *gradient);
    void *internal_data;
}optimizer;
#endif //BRAIN_CRAFT_SRC_NEURAL_BFO_OPTIMIZERS_OPTIMIZER_H
