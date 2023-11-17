cmake_minimum_required(VERSION 3.26)
project(T2 C)

set(CMAKE_C_STANDARD 99)

find_library(Semaphoren libSemaphoren.so)

add_executable(T2
        testat2_a1.c
        testat2_a1_with_libary.c
)

target_link_libraries(T2 ${Semaphoren})
