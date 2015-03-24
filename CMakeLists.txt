# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# Workspace name
project(cell-sim)

option(ASAN "If address sanitizer should be used" Off)
option(MSAN "If memory sanitizer should be used" Off)
option(USE_LIBCPP "If libc++ should be used" Off)
option(ENABLE_GUI "Enable GUI" On)
option(ENABLE_RENDER "Enable simulation rendering" On)

# JPEG support
find_package(JPEG)

if (JPEG_FOUND)
    message("-- JPEG support enabled")
    add_definitions(-DJPEG_SUPPORT)
endif ()

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
    message("-- Using address sanitizer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
elseif (MSAN)
    message("-- Using memory sanitizer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
endif ()

if (ENABLE_GUI)
    message("-- GUI enabled")
    set(ENABLE_RENDER 1)
    add_definitions(-DENABLE_GUI=1)
endif ()

if (ENABLE_RENDER)
    message("-- OpenGL rendering enabled")
    add_definitions(-DENABLE_RENDER=1)
endif ()

# Projects
add_subdirectory(core)

if (ENABLE_RENDER)
    add_subdirectory(render)
endif ()

add_subdirectory(simulator)

if (ENABLE_GUI)
    add_subdirectory(gui)
else ()
    add_subdirectory(cli)
endif ()

# ######################################################################### #
