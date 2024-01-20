//
// Created by kosenko on 20.01.24.
//

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "tests.h"
#include "../libraries/math_bfo/matrix_operations.h"

double get_time_of_function_work(void f(void*), void* data_for_func)
{
    time_t now, end;
    time(&now);
    f(data_for_func);
    time(&end);
    return (double)(end - now);
}

void print_time_of_function_work(void f(void*), void* data_for_func)
{
    double time = get_time_of_function_work(f, data_for_func);
    printf("Time of working is %f\n", time);
}


void matrix_multiplication_voidp(void* matrixes){
    matrix** mp = matrixes;
    matrix_multiplication(*(mp[0]), *(mp[1]));
}

void matrix_multiplication_time_checking(matrix matrix1, matrix matrix2){
    void** matrix_voidp = calloc(2, sizeof(matrix*));
    matrix_voidp[0] = &matrix1;
    matrix_voidp[1] = &matrix2;
    print_time_of_function_work(matrix_multiplication_voidp, matrix_voidp);
    free(matrix_voidp);
}
void check_matrix_multiplication() {
    double m[2][3] = {{1.1, 2.2, 5.1},
                      {3.2, 4.1, 6.5}};
    int size1 = sizeof(m) / sizeof(m[0]);
    int size2 = sizeof(m[0]) / sizeof(m[0][0]);
    double **matrix_pointer;
    matrix_pointer = calloc(size1, sizeof(double *));
    for (int i = 0; i < size1; ++i) {
        matrix_pointer[i] = calloc(size2, sizeof(double));
    }
    for (int i = 0; i < size1; ++i) {
        for (int j = 0; j < size2; ++j) {
            matrix_pointer[i][j] = m[i][j];
        }
    }
    matrix checking;
    checking.table = matrix_pointer;
    checking.i = size1;
    checking.j = size2;
    matrix checking1 = matrix_transposition(checking);
    matrix_multiplication_time_checking(checking, checking1);
    matrix dop = matrix_copy(checking);

    matrix_free(checking);
    matrix_free(checking1);

    double m1[3][2] = {{1, 2},
                       {3, 4},
                       {5, 6}};
    size1 = sizeof(m1) / sizeof(m1[0]);
    size2 = sizeof(m1[0]) / sizeof(m1[0][0]);
    matrix_pointer = calloc(size1, sizeof(double *));
    for (int i = 0; i < size1; ++i) {
        matrix_pointer[i] = calloc(size2, sizeof(double));
    }
    for (int i = 0; i < size1; ++i) {
        for (int j = 0; j < size2; ++j) {
            matrix_pointer[i][j] = m1[i][j];
        }
    }
    checking.table = matrix_pointer;
    checking.i = size1;
    checking.j = size2;
    checking1 = matrix_transposition(checking);
    matrix_multiplication_time_checking(checking, checking1);
    matrix_multiplication_time_checking(checking, dop);

    matrix_free(checking);
    matrix_free(checking1);
    matrix_free(dop);
}