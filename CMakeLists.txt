# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# Workspace name
project(cell-sim)

# Options
option(ENABLE_GUI                   "Enable GUI" On)
option(ENABLE_CLI                   "Enable CLI" Off)
option(ENABLE_RENDER                "Enable simulation rendering" On)
option(ENABLE_SSE                   "Enable SSE instructions" On)
option(ENABLE_BUILDIN_PHYSICS       "Enable build-in physics engine (Box2D)" On)
option(ENABLE_MEASUREMENT           "Enable or disable time measurement" Off)
option(ENABLE_BUILDIN_PHYSICS_DEBUG "Enable or disable physics engine debugging" Off)

option(BUILD_TOOLS                  "Build support tools" Off)
set(MODULES                         "streamlines;diffusion;diffusion-streamlines;cell;picture" CACHE STRING "A list of build modules")
set(MODULES_BUILDIN                 "streamlines;diffusion;diffusion-streamlines;cell;picture" CACHE STRING "A list of build-in modules")

# Options specific for different compilers
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    option(CLANG_ASAN               "If address sanitizer should be used" Off)
    option(CLANG_MSAN               "If memory sanitizer should be used" Off)
    option(CLANG_USE_LIBCPP         "If libc++ should be used" Off)

    # C++11 required
    add_compile_options(-pedantic-errors -pedantic -std=c++11)

    # Use libc++
    if (CLANG_USE_LIBCPP)
        add_compile_options(-stdlib=libc++)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    endif()

    # Debug flags
    if (CLANG_ASAN)
        message("-- Using address sanitizer")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    elseif (CLANG_MSAN)
        message("-- Using memory sanitizer")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=memory -fno-omit-frame-pointer")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC

    # C++11 required
    add_compile_options(-pedantic-errors -pedantic -std=c++11)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
endif()

# Default include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# GUI enabled
if (ENABLE_GUI)
    message("-- Build GUI application")
    set(ENABLE_RENDER 1)
else ()
    # If GUI is disabled build CLI
    set(ENABLE_CLI 1)
endif ()

# CLI enabled
if (ENABLE_CLI)
    message("-- Build CLI application")
endif ()

# Rendering enabled
if (ENABLE_RENDER)
    message("-- OpenGL rendering support")
    add_definitions(-DENABLE_RENDER=1)
endif ()

if (ENABLE_SSE)
    # TODO: detect SSE support
    message("-- Use SSE instructions")
    add_definitions(-DENABLE_SSE=1)
endif ()

# Build-in physics enabled
if (ENABLE_BUILDIN_PHYSICS)
    message("-- Using build-in physics (Box2D)")
    # Physics engine
    include_directories(Box2D)
    add_definitions(-DENABLE_PHYSICS=1)

    if (ENABLE_BUILDIN_PHYSICS_DEBUG)
        message("-- Physics engine debugging enabled")
        add_definitions(-DENABLE_PHYSICS_DEBUG=1)
    endif ()
endif ()

if (ENABLE_MEASUREMENT)
    message("-- Time measurement enabled")
    add_definitions(-DENABLE_MEASUREMENT=1)
endif ()

# Default libraries
add_subdirectory(core)
add_subdirectory(parser)
add_subdirectory(parser-xml)

# Add render library
if (ENABLE_RENDER)
    add_subdirectory(render)
endif ()

if (ENABLE_BUILDIN_PHYSICS)
    # Physics engine
    add_subdirectory(Box2D)

    # Link libraries
    link_directories(${CMAKE_BINARY_DIR}/Box2D/Box2D)
endif ()

# Main simulator project
add_subdirectory(simulator)

# Modules
foreach (MODULE ${MODULES})
    message("-- Module: ${MODULE}")
    add_subdirectory(modules/${MODULE})
endforeach ()

# Build GUI project
if (ENABLE_GUI)
    add_subdirectory(gui)
endif ()

# Build CLI application
add_subdirectory(cli)

if (BUILD_TOOLS)
    add_subdirectory(tools/shader-convert)
endif()

# ######################################################################### #
