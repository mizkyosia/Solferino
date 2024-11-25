# Projects Settings
cmake_minimum_required (VERSION 3.15)
project (Solferino)

# The main executable
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
    )

add_executable(Solferino
    ${SOURCES})

# Copy assets in build directory
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets")

# Compil options
set_property(TARGET Solferino PROPERTY CXX_STANDARD 20)
target_compile_options(Solferino PRIVATE -Wall)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)

# Libraries
target_link_libraries(Solferino
    PRIVATE glfw
    PRIVATE libglew_static
    PRIVATE glm
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/asset.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/util/asset.hpp
)
target_include_directories(Solferino
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)
