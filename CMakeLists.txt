# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# Workspace name
project(cell-sim)

option(ASAN "If address sanitizer should be used" Off)
option(MSAN "If memory sanitizer should be used" Off)
option(USE_LIBCPP "If libc++ should be used" Off)

# Default include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# C++11 required
add_compile_options(-pedantic-errors -pedantic -std=c++11)

# Use libc++
if (USE_LIBCPP)
    add_compile_options(-stdlib=libc++)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
endif()

# Debug flags
if (ASAN)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
elseif (MSAN)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
endif ()

# Projects
add_subdirectory(core)
add_subdirectory(render)
add_subdirectory(physics)
add_subdirectory(simulator)
add_subdirectory(javascript)
add_subdirectory(gui)

# ######################################################################### #
