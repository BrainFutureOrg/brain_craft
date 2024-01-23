//
// Created by maximus on 23.01.24.
//

#include <stdlib.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <stdio.h>
#include "matrix_operations.h"

typedef enum {
    ONE_MATRIX_MULTITHREAD,
    TWO_MATRIX_MULTITHREAD,
} parallel_operation_type;



typedef struct {
    int start_i, start_j, end_i, end_j;
} matrix_multithread_start_end_numbers;

typedef struct{
    void* struct_for_operation;
    parallel_operation_type type;
    matrix_multithread_start_end_numbers numbers;
} parallel_matrix_data;

typedef struct{
    void (*f)(matrix*, matrix*, int, int);
    matrix *m, *result;
} one_matrix_operation;

typedef struct{
    void (*f)(matrix*, matrix*, matrix*, int, int);
    matrix *m1, *m2, *result;
} two_matrix_operation;

int get_j_size(parallel_matrix_data *data)
{
    switch (data->type)
    {
        case ONE_MATRIX_MULTITHREAD:
            return ((one_matrix_operation*) data->struct_for_operation)->m->j;
        case TWO_MATRIX_MULTITHREAD:
            return ((two_matrix_operation*) data->struct_for_operation)->m2->j;
    }
}

void* matrix_multithread_operation(void* data){
    parallel_matrix_data *data_unpacked = data;
    int j_size = get_j_size(data_unpacked);
    for (int i = data_unpacked->numbers.start_i; i <= data_unpacked->numbers.end_i; ++i)
    {
        int j_start = 0;
        if(i == data_unpacked->numbers.start_i)
        {
            j_start = data_unpacked->numbers.start_j;

        }
        if(i == data_unpacked->numbers.end_i)
        {
            j_size = data_unpacked->numbers.start_j;
        }
        for (int j = j_start; j <= j_size; ++j)
        {
            switch (data_unpacked->type)
            {
                case ONE_MATRIX_MULTITHREAD:;
                    one_matrix_operation* data_full_unpacked1 = (one_matrix_operation *)data_unpacked;
                    data_full_unpacked1->f(data_full_unpacked1->result, data_full_unpacked1->m, i, j);
                    break;
                case TWO_MATRIX_MULTITHREAD:;
                    two_matrix_operation* data_full_unpacked2 = (two_matrix_operation *)data_unpacked;
                    data_full_unpacked2->f(data_full_unpacked2->result, data_full_unpacked2->m1, data_full_unpacked2->m2, i, j);
                    break;
            }
        }
    }
    return NULL;
}

matrix matrix_multiplication_parallel(matrix first_matrix, matrix second_matrix)
{
    int core_num = get_nprocs();
    matrix result = matrix_creation(first_matrix.i, second_matrix.j);
    int num_of_matrix_elements = result.i * result.j;
    if(num_of_matrix_elements < core_num)
    {
        core_num = num_of_matrix_elements;
    }
    int step_size = num_of_matrix_elements / core_num;
    step_size += num_of_matrix_elements % core_num ? 1 : 0;
    int elem = 0;
    pthread_t *thread = calloc(core_num, sizeof(pthread_t));
    two_matrix_operation operation;
    operation.m1 = &first_matrix;
    operation.m2 = &second_matrix;
    operation.result = &result;
    for (int i=0; i<core_num; i++){
        parallel_matrix_data *data = calloc(1, sizeof(parallel_matrix_data));
        data->type = TWO_MATRIX_MULTITHREAD;
        data->struct_for_operation = &operation;
        int new_elem = elem + step_size;
        data->numbers = (matrix_multithread_start_end_numbers){elem % result.i, elem / result.i,
                                                               new_elem % result.i, new_elem / result.i};

        pthread_create(&thread[i], NULL, matrix_multithread_operation, (void *)data);
    }
    for (int i = 0; i < core_num; ++i)
    {
        pthread_join(thread[i], NULL);
        printf("Thread finished");
    }

    return result;
}
