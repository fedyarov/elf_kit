cmake_minimum_required(VERSION 3.0.0)

project(elfkit_ld)

set(CMAKE_C_STANDARD 11)

add_executable(elfkit_ld main.c elf.c debug_msgs.c)