cmake_minimum_required(VERSION 3.26)
project(T1 C)

set(CMAKE_C_STANDARD 99)

add_executable(T1 testat1.c
        ../T2/testat2_a1_with_libary.c
        ../T2/testat2_a2.c
        ../T2/testat2_a1.c)
