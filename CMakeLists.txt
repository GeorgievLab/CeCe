# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# Workspace name
project(cell-sim)

option(ASAN "If address sanitizer should be used" Off)
option(MSAN "If memory sanitizer should be used" Off)
option(USE_LIBCPP "If libc++ should be used" Off)
option(ENABLE_GUI "Enable GUI" On)
option(ENABLE_RENDER "Enable simulation rendering" On)
option(ENABLE_SSE "Enable SSE instructions" On)
option(INCLUDE_SOURCE "Include source code in object files" Off)
option(NATIVE_BUILD "If sources should be build for current CPU -march=native" Off)
option(USE_BUILDIN_PHYSICS "Enable build-in physics engine (Box2D)" On)
option(MODULE_BUILD_STATIC "Build modules as static libraries" Off)
option(ENABLE_MEASUREMENT "Enable or disable time measurement" Off)
option(ENABLE_PHYSICS_DEBUG "Enable or disable physics engine debugging" Off)

# Default include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# C++11 required
add_compile_options(-pedantic-errors -pedantic -std=c++11)

if (INCLUDE_SOURCE)
    add_compile_options(-g)
endif ()

if (NATIVE_BUILD)
    message("-- Native architecture build")
    add_compile_options(-march=native)
endif ()

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

if (ENABLE_SSE)
    message("-- Use SSE instructions")
    add_definitions(-DENABLE_SSE=1)
endif ()

if (USE_BUILDIN_PHYSICS)
    message("-- Using build-in physics")
    # Physics engine
    include_directories(Box2D)
    add_definitions(-DENABLE_PHYSICS=1)

    if (ENABLE_PHYSICS_DEBUG)
        message("-- Physics engine debugging enabled")
        add_definitions(-DENABLE_PHYSICS_DEBUG=1)
    endif ()
endif ()

if (ENABLE_MEASUREMENT)
    message("-- Time measurement enabled")
    add_definitions(-DENABLE_MEASUREMENT=1)
endif ()

# Projects
add_subdirectory(core)
add_subdirectory(parser)
add_subdirectory(parser-xml)

if (ENABLE_RENDER)
    add_subdirectory(render)
endif ()

if (USE_BUILDIN_PHYSICS)
    # Physics engine
    add_subdirectory(Box2D)

    # Link libraries
    link_directories(${CMAKE_BINARY_DIR}/Box2D/Box2D)
endif ()

add_subdirectory(simulator)

# Modules
add_subdirectory(modules/streamlines)
add_subdirectory(modules/diffusion)
add_subdirectory(modules/diffusion-streamlines)
add_subdirectory(modules/cell)
add_subdirectory(modules/picture)

if (ENABLE_GUI)
    add_subdirectory(gui)
else ()
    add_subdirectory(cli)
endif ()

# ######################################################################### #
