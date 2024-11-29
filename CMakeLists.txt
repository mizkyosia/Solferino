# Projects Settings
cmake_minimum_required(VERSION 3.15)
project(Solferino)

# Copy assets in build directory
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets")

# Include raylib
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if(NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if(NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

link_directories(lib/imgui)

# The main executable
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

set(RLIMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/rlImGui/rlImGui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/rlImGui/imgui_impl_raylib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/rlImGui/rlImGui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/rlImGui/rlImGuiColors.h
)

add_executable(Solferino
    ${SOURCES}
    ${RLIMGUI_SOURCES}
)

# Compil options
set_property(TARGET Solferino PROPERTY CXX_STANDARD 20)
target_compile_options(Solferino PRIVATE -Wall)

add_library(rlImGui STATIC ${RLIMGUI_SOURCES})

target_link_libraries(Solferino PRIVATE raylib imgui)

# Include imgui
target_include_directories(Solferino
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/rlImGui
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)
