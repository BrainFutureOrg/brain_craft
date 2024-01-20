//
// Created by kosenko on 07.04.23.
//

#ifndef C_VERSION_STATISTICAL_RANDOM_H
#define C_VERSION_STATISTICAL_RANDOM_H

#include <limits.h>


#define randint(a, b) (random() % (b-a) + a)
#define rand_plain ((double) random() / INT_MAX)

double randn();

double randu_range(double start, double end);

#endif //C_VERSION_STATISTICAL_RANDOM_H
