cmake_minimum_required(VERSION 3.9)
project(l13_semaphores2 C)

set(CMAKE_C_STANDARD 99)

add_executable(l13_semaphores2 main.c)

target_link_libraries(l13_semaphores2 "pthread")