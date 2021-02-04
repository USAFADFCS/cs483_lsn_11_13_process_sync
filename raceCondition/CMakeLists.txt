cmake_minimum_required(VERSION 3.9)
project(l11_racecondition C)

set(CMAKE_C_STANDARD 99)

add_executable(l11_racecondition main.c)

target_link_libraries(l11_racecondition "-lpthread")