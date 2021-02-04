cmake_minimum_required(VERSION 3.9)
project(l13_producer_consumer C)

set(CMAKE_C_STANDARD 99)

add_executable(l13_producer_consumer main.c)

target_link_libraries(l13_producer_consumer "pthread")