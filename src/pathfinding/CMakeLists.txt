cmake_minimum_required (VERSION 3.12)

project(Solferino)

list(APPEND PROJECT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/Node.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Graph.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Node.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Graph.hpp
)