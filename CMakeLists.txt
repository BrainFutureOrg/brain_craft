cmake_minimum_required(VERSION 3.27)
project(brain_craft C)

set(CMAKE_C_STANDARD 11)

add_executable(brain_craft
        src/main.c
        src/libraries/math_bfo/matrix_operations.c
        src/libraries/math_bfo/batch_operations.c
        src/libraries/loging_bfo/log.c
        src/libraries/tests_bfo/test_funcs.c
        src/libraries/math_bfo/random_bfo/statistical_random.c
        src/libraries/math_bfo/print/my_print.c
        src/tests/tests.c
        src/neural_bfo/node.c)