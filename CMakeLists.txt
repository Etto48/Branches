cmake_minimum_required(VERSION 3.13)
project(branches)

set(CMAKE_CXX_STANDARD 20)

add_executable(branches main.cpp branches_core.h branches_core.cpp graph.h)