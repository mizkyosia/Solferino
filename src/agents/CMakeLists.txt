cmake_minimum_required (VERSION 3.12)

project(Solferino)

list(APPEND PROJECT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/Vehicle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Car.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Vehicle.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Car.hpp
)