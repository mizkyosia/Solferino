cmake_minimum_required (VERSION 3.12)

set(PROJECT_SRCS "main.cpp" "MyApplication.cpp" "MyApplication.hpp")

include("agents/CMakeLists.txt")
include("pathfinding/CMakeLists.txt")
include("util/CMakeLists.txt")

add_executable(Solferino ${PROJECT_SRCS})