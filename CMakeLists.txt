cmake_minimum_required(VERSION 3.13)
project(branches)

set(CMAKE_CXX_STANDARD 20)

add_executable(branches
        resources/res.rc
        main.cpp
        branches_core/algebraParser/algebraParser.cpp branches_core/algebra_tools_/algebra_tools_.cpp branches_core/varNode/varNode.cpp branches_core/algebraNode/algebraNode.cpp branches_core/funcNode/funcNode.cpp branches_core/exprNode/exprNode.cpp graphics/point.h branches_core/vectorParser/vectorParser.cpp branches_core/vectorParser/vectorParser.h branches_core/DataTypes/DataTypes.h branches_core/DataTypes/vec.h branches_core/DataTypes/mat.h branches_core/algebra_tools_/exception.h functions/numericIntegration.h graphics/colors.h graphics/winPC3d.h graphics/winPS3d.h branches_core/prefixNode/prefixNode.cpp branches_core/prefixNode/prefixNode.h)
