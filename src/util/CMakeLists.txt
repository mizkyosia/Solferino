cmake_minimum_required (VERSION 3.12)

project(Solferino)

list(APPEND PROJECT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/Misc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Misc.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Vec2.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Vec2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Shader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Shader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/glError.cpp
    ${CMAKE_CURRENT_LIST_DIR}/glError.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Application.hpp
)