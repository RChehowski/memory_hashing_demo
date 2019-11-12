cmake_minimum_required(VERSION 3.13)
project(CPPDemo)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(CPPDemo main.c source/sha256.c source/mumbo_jumbo.c)