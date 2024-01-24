//
// Created by maximus on 21.01.23.
//
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix_operations.h"
#include <math.h>
#include <stdarg.h>
#include "print/my_print.h"

void matrix_multiplication_one_entity(matrix* first_matrix, matrix *second_matrix, matrix *result, int i, int j){
    result->table[i][j] = 0;
    for (int i2 = 0; i2 < first_matrix->j; i2++) {
        result->table[i][j] += first_matrix->table[i][i2] * second_matrix->table[i2][j];
    }
}

matrix matrix_multiplication_simple(matrix first_matrix, matrix second_matrix) {
    matrix result = matrix_creation(first_matrix.i, second_matrix.j);
//#pragma omp parallel shared(first_matrix, second_matrix, result) default(none)
    {
//#pragma omp for
        for (int i = 0; i < first_matrix.i; i++) {
            for (int j = 0; j < second_matrix.j; j++) {
                matrix_multiplication_one_entity(&first_matrix, &second_matrix, &result, i, j);
            }
        }
    }
    return result;
}

matrix matrix_multiplication_winograd(matrix first_matrix, matrix second_matrix) {
    int n = first_matrix.i, k = first_matrix.j;
    int m = second_matrix.j;
    matrix result = matrix_creation(n, m);

    double *eta = calloc(m, sizeof(double));
    for (int j = 0; j < m; j++) {
        for (int k2 = 0; k2 < k / 2; k2++) {
            eta[j] = 0;
            eta[j] += second_matrix.table[k2 * 2][j] * second_matrix.table[k2 * 2 + 1][j];
        }
    }
//#pragma omp parallel shared(first_matrix, second_matrix, result, eta, n, m, k) default(none)
    {
//#pragma omp for
        for (int i = 0; i < n; i++) {
            double zeta = 0;
            for (int k2 = 0; k2 < k / 2; k2++) {
                zeta += first_matrix.table[i][k2 * 2] * first_matrix.table[i][k2 * 2 + 1];
            }
            for (int j = 0; j < m; j++) {

                double result_number = 0;
                for (int k2 = 0; k2 < k / 2; k2++) {
                    result_number += (first_matrix.table[i][k2 * 2] + second_matrix.table[k2 * 2 + 1][j]) *
                                     (first_matrix.table[i][k2 * 2 + 1] + second_matrix.table[k2 * 2][j]);
                }
                result.table[i][j] = result_number - zeta - eta[j] +
                                     (k % 2 != 0 ? first_matrix.table[i][k - 1] * second_matrix.table[k - 1][j]
                                                 : 0);
            }
        }
    }
    free(eta);
    return result;
}

matrix matrix_multiplication(matrix first_matrix, matrix second_matrix) {
    matrix result;
    if (first_matrix.j != second_matrix.i) {
        printf("ERRNO (matr mult firstmatrix j = %d, secondmatrix i = %d)", first_matrix.j, second_matrix.i);
        errno = ERANGE;
        result.i = result.j = 0;
        return result;
    }
//    result = matrix_multiplication_winograd(first_matrix, second_matrix);
    result = matrix_multiplication_simple(first_matrix, second_matrix);
    return result;
}

double matrix_get_element(matrix matrx, int i, int j) {
    return matrx.table[i][j];
}

matrix matrix_addition(matrix first_matrix, matrix second_matrix) {
    matrix result;
    if (first_matrix.i != second_matrix.i || first_matrix.j != second_matrix.j) {
        printf("matr add firstmatrix %d x %d secondmatrix %d x %d", first_matrix.i, first_matrix.j, second_matrix.i,
               second_matrix.j);
        errno = ERANGE;
        result.i = result.j = 0;
        return result;
    }
    result = matrix_copy(first_matrix);
    for (int i = 0; i < first_matrix.i; i++) {
        for (int j = 0; j < first_matrix.j; j++) {
            result.table[i][j] += second_matrix.table[i][j];
        }
    }
    return result;
}

void matrix_addition_inplace(matrix changed_matrix, matrix second_matrix) {
    if (changed_matrix.i != second_matrix.i || changed_matrix.j != second_matrix.j) {
        printf("matr add firstmatrix %d x %d secondmatrix %d x %d", changed_matrix.i, changed_matrix.j, second_matrix.i,
               second_matrix.j);
        errno = ERANGE;
    }
    for (int i = 0; i < changed_matrix.i; i++) {
        for (int j = 0; j < changed_matrix.j; j++) {
            changed_matrix.table[i][j] += second_matrix.table[i][j];
        }
    }
}

matrix matrix_creation(int i, int j) {
    matrix result;
    result.i = i;
    result.j = j;
    result.table = calloc(i, sizeof(double *));
    for (int iterator = 0; iterator < i; iterator++) {
        result.table[iterator] = calloc(j, sizeof(double));
    }
    return result;
}

matrix matrix_copy(matrix matrix_to_copy) {
    matrix copy = matrix_creation(matrix_to_copy.i, matrix_to_copy.j);
    for (int i = 0; i < matrix_to_copy.i; i++) {
        for (int j = 0; j < matrix_to_copy.j; j++) {
            copy.table[i][j] = matrix_to_copy.table[i][j];
        }
    }
    return copy;
}

matrix matrix_copy_activated(matrix matrix_to_copy, void (*active_func)(matrix *)) {
    matrix copy = matrix_copy(matrix_to_copy);
    active_func(&copy);
    return copy;
}

matrix matrix_transposition(matrix matrix_to_transpose) {
    matrix result_matrix;
    result_matrix = matrix_creation(matrix_to_transpose.j, matrix_to_transpose.i);
    for (int i = 0; i < matrix_to_transpose.i; i++) {
        for (int j = 0; j < matrix_to_transpose.j; j++) {
            result_matrix.table[j][i] = matrix_to_transpose.table[i][j];
        }
    }
    return result_matrix;
}

void matrix_print(matrix matrix_to_print) {
    for (int i = 0; i < matrix_to_print.i; i++) {
        for (int j = 0; j < matrix_to_print.j; j++) {
            printf("%.2f ", matrix_to_print.table[i][j]);
        }
        printf("\n");
    }
}

void matrix_print_with_indexation(matrix matrix_to_print, int integer_allocate, int fraction_allocate) {
    printf("    ");
    int len = integer_allocate + fraction_allocate;
    for (int j = 0; j < matrix_to_print.j; j++) {
        print_double_number(j, len, 0);
        putchar(' ');
    }
    printf("\n   ");
    for (int j = 0; j < matrix_to_print.j; j++) {
        for (int i = 0; i < len + 1; ++i) {
            putchar('_');
        }
    }
    printf("\n");

    for (int i = 0; i < matrix_to_print.i; i++) {
        printf("%2d| ", i);
        for (int j = 0; j < matrix_to_print.j; j++) {
            print_double_number(matrix_to_print.table[i][j], integer_allocate, fraction_allocate);
            putchar(' ');
        }
        printf("\n");
    }
}

void matrix_free(matrix matrix_to_free) {
    for (int i = 0; i < matrix_to_free.i; ++i) {
        free(matrix_to_free.table[i]);
    }
    free(matrix_to_free.table);
}

void matrix_free_arrayed(matrix *matrix_array_to_free, int array_size) {
    for (int i = 0; i < array_size; i++) {
        matrix_free(matrix_array_to_free[i]);
    }
    free(matrix_array_to_free);
}

void matrix_function_to_elements(matrix matrix_for_operation, double (*func)(double)) {
    for (int i = 0; i < matrix_for_operation.i; i++) {
        for (int j = 0; j < matrix_for_operation.j; j++) {
            matrix_for_operation.table[i][j] = func(matrix_for_operation.table[i][j]);
        }
    }
}

void matrix_multiply_by_constant(matrix matrix_for_operation, double number) {
    for (int i = 0; i < matrix_for_operation.i; i++) {
        for (int j = 0; j < matrix_for_operation.j; j++) {
            matrix_for_operation.table[i][j] = number * matrix_for_operation.table[i][j];
        }
    }
}

void matrix_add_scalar(matrix matrix_for_operation, double scalar) {
    for (int i = 0; i < matrix_for_operation.i; i++) {
        for (int j = 0; j < matrix_for_operation.j; j++) {
            matrix_for_operation.table[i][j] += scalar;
        }
    }
}

matrix make_matrix_from_array(const double *double_array, int i, int j) {
    matrix result;
    result = matrix_creation(i, j);
    for (int iterator = 0; iterator < i; iterator++) {
        for (int iterator1 = 0; iterator1 < j; iterator1++) {
            result.table[iterator][iterator1] = *(double_array + iterator * j + iterator1);
        }
    }
    return result;
}

matrix matrix_substact(matrix first_matrix, matrix second_matrix) {
    matrix result;
    if (first_matrix.i != second_matrix.i || first_matrix.j != second_matrix.j) {
        printf("matr sub firstmatrix %d x %d secondmatrix %d x %d", first_matrix.i, first_matrix.j, second_matrix.i,
               second_matrix.j);
        errno = ERANGE;
        result.i = result.j = 0;
        return result;
    }
    result = matrix_copy(first_matrix);
    for (int i = 0; i < first_matrix.i; i++) {
        for (int j = 0; j < first_matrix.j; j++) {
            result.table[i][j] -= second_matrix.table[i][j];
        }
    }
    return result;
}

void matrix_subtract_inplace(matrix changed_matrix, matrix second_matrix) {
    if (changed_matrix.i != second_matrix.i || changed_matrix.j != second_matrix.j) {
        printf("matr sub firstmatrix %d x %d secondmatrix %d x %d", changed_matrix.i, changed_matrix.j, second_matrix.i,
               second_matrix.j);
        errno = ERANGE;
    }
    for (int i = 0; i < changed_matrix.i; i++) {
        for (int j = 0; j < changed_matrix.j; j++) {
            changed_matrix.table[i][j] -= second_matrix.table[i][j];
        }
    }
}

matrix matrix_multiplication_elements(matrix first_matrix, matrix second_matrix) {
    matrix result;
    result = matrix_copy(first_matrix);
    for (int i = 0; i < first_matrix.i; ++i) {
        for (int j = 0; j < first_matrix.j; ++j) {
            result.table[i][j] *= second_matrix.table[i][j];
        }
    }
    return result;
}

void matrix_multiplication_elements_inplace(matrix first_matrix, matrix second_matrix)
{
    for (int i = 0; i < first_matrix.i; ++i) {
        for (int j = 0; j < first_matrix.j; ++j) {
            first_matrix.table[i][j] *= second_matrix.table[i][j];
        }
    }
}

matrix create_vector(int feach_num, int this) {
    matrix result = matrix_creation(feach_num, 1);
    for (int i = 0; i < feach_num; ++i) {
        result.table[i][0] = i == this ? 1 : 0;
    }
    return result;
}

void matrix_restrict(matrix matrix_for_operation, double limit) {
    for (int i = 0; i < matrix_for_operation.i; i++) {
        for (int j = 0; j < matrix_for_operation.j; j++) {
            if (matrix_for_operation.table[i][j] > limit)matrix_for_operation.table[i][j] = limit;
            if (matrix_for_operation.table[i][j] < -limit)matrix_for_operation.table[i][j] = -limit;
        }
    }
}

double frobenius_norm(matrix matrix_for_operation) {
    double sum = 0;
    for (int i = 0; i < matrix_for_operation.i; i++) {
        for (int j = 0; j < matrix_for_operation.j; j++) {
            sum += pow(matrix_for_operation.table[i][j], 2);
        }
    }
    return sqrt(sum);
}

void frobenius_normalize(matrix matrix_for_operation) {
    matrix_multiply_by_constant(matrix_for_operation, 1 / frobenius_norm(matrix_for_operation));
}

double matrix_max_absolute(matrix matrix_for_operation) {
    double max = fabs(matrix_for_operation.table[0][0]);
    for (int i = 0; matrix_for_operation.i; i++) {
        for (int j = 0; j < matrix_for_operation.j; j++) {
            if (max < fabs(matrix_for_operation.table[i][j]))max = fabs(matrix_for_operation.table[i][j]);
        }
    }
    return max;
}

void max_abs_normalize(matrix matrix_for_operation) {
    matrix_multiply_by_constant(matrix_for_operation, 1 / matrix_max_absolute(matrix_for_operation));
}

coordinates matrix_argmax(matrix matrix_for_operation) {
    coordinates result;
    result.i = result.j = 0;
    double max_elem = matrix_get_element(matrix_for_operation, 0, 0);
    for (int i = 0; i < matrix_for_operation.i; i++) {
        for (int j = 0; j < matrix_for_operation.j; j++) {
            double element = matrix_get_element(matrix_for_operation, i, j);
            if (element > max_elem) {
                max_elem = element;
                result.i = i;
                result.j = j;
            }
        }
    }
    return result;
}

char coordinates_equals(coordinates coordinates1, coordinates coordinates2) {
    return coordinates1.i == coordinates2.i && coordinates1.j == coordinates2.j ? 1 : 0;
}

matrix matrix_average(int num, matrix *matrixes_for_operation) {
    matrix result = matrix_creation(matrixes_for_operation[0].i, matrixes_for_operation[0].j);
    for (int i = 0; i < num; i++) {
        matrix_addition_inplace(result, matrixes_for_operation[i]);
    }
    matrix_multiply_by_constant(result, 1. / num);
    return result;
}

void matrix_fill_edges_0_inplace(matrix matrix_for_operation) {
    for (int i = 0; i < matrix_for_operation.i; i++) {
        if ((i - 7) % 28 >= 14)matrix_for_operation.table[i][0] = 0;
    }
}

void matrix_cut_edges_inplace(matrix *matrix_for_operation) {
    int i2 = 0;
    /*double** newtable= calloc(matrix_for_operation.i/2, sizeof(double*));
    for(int i=0; i<matrix_for_operation.i/2; i++){
        newtable[i]= calloc(1, sizeof(double ));
    }*/
    matrix newmatrix = matrix_creation(matrix_for_operation->i / 2, 1);
    for (int i = 0; i < matrix_for_operation->i; i++) {
        if (i % 28 > 7 && i % 28 < 21) {
            //matrix_for_operation.table[i][0] = 0;
            newmatrix.table[i2][0] = matrix_for_operation->table[i][0];
            i2++;
        }
    }
    matrix_free(*matrix_for_operation);
    *matrix_for_operation = newmatrix;
}