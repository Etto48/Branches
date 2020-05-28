cmake_minimum_required(VERSION 3.13)
project(branches)

set(CMAKE_CXX_STANDARD 20)
#branches_core/algebraParser/algebraParser.h graph.h branches_core/branches_core.h branches_core/exprNode/exprNode.h branches_core/funcNode/funcNode.h branches_core/varNode/varNode.h branches_core/algebraNode/algebraNode.h branches_core/algebra_tools_/algebra_tools_.h
add_executable(branches main.cpp branches_core/algebraParser/algebraParser.cpp branches_core/algebra_tools_/algebra_tools_.cpp branches_core/varNode/varNode.cpp branches_core/algebraNode/algebraNode.cpp branches_core/funcNode/funcNode.cpp branches_core/exprNode/exprNode.cpp)