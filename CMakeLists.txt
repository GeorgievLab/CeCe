# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# ######################################################################### #

# Workspace name
project(cell-sim)

# Add path to additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

# Applicatio version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 1)
set(VERSION_SPECIAL "alpha")

# Setup macros with application version
add_definitions(
    -DVERSION_MAJOR=${VERSION_MAJOR}
    -DVERSION_MINOR=${VERSION_MINOR}
    -DVERSION_PATCH=${VERSION_PATCH}
    -DVERSION_SPECIAL="${VERSION_SPECIAL}"
)

# Directory with plugins
# TODO: change to plugins (after directory rename)
set(DIR_PLUGINS "libraries")

# Options
option(ENABLE_GUI                   "Enable GUI" On)
option(ENABLE_CLI                   "Enable CLI" Off)
option(ENABLE_RENDER                "Enable simulation rendering" On)
option(ENABLE_SSE                   "Enable SSE instructions" On)
option(ENABLE_BUILDIN_PHYSICS       "Enable build-in physics engine (Box2D)" On)
option(ENABLE_MEASUREMENT           "Enable or disable time measurement" Off)
option(ENABLE_BUILDIN_PHYSICS_DEBUG "Enable or disable physics engine debugging" Off)
option(ENABLE_PLUGINS_DIRECT_LINK   "Enable or disable plugins direct linking into executables (useful for testing)" Off)
option(BUILD_TESTS                  "Build simulator part tests (requires GTest)" Off)
option(BUILD_TOOLS                  "Build support tools" Off)

# List of plugins values
set(PLUGINS_AVAILABLE "")
set(PLUGINS_BUILD "")
set(PLUGINS_BUILDIN "")

# Get list of available plugins
file(GLOB PLUGINS_DIRECTORIES RELATIVE "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}" "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}/*")
foreach(PLUGIN_DIR ${PLUGINS_DIRECTORIES})
    if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}/${PLUGIN_DIR}")
        list(APPEND PLUGINS_AVAILABLE ${PLUGIN_DIR})
        option(PLUGINS_BUILD_${PLUGIN_DIR} "Build plugin '${PLUGIN_DIR}'" On)
        option(PLUGINS_BUILDIN_${PLUGIN_DIR} "Build plugin '${PLUGIN_DIR}' as build-in plugin" Off)

        if (PLUGINS_BUILDIN_${PLUGIN_DIR})
            list(APPEND PLUGINS_BUILDIN ${PLUGIN_DIR})
            set(PLUGINS_BUILD_${PLUGIN_DIR} On)
        endif ()

        if (PLUGINS_BUILD_${PLUGIN_DIR})
            list(APPEND PLUGINS_BUILD ${PLUGIN_DIR})
        endif ()
    endif()
endforeach()

# ######################################################################### #

# Setup compiler specific things
include(Compilers)

# Library of functions
include(Functions)

# ######################################################################### #

# Default include directories
include_directories(${CMAKE_SOURCE_DIR})

# Boost libraries
set(Boost_USE_STATIC_LIBS On)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

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
    find_package(BOX2D REQUIRED)
    include_directories(${BOX2D_INCLUDE_DIR})

    add_definitions(-DENABLE_PHYSICS=1)

    if (ENABLE_BUILDIN_PHYSICS_DEBUG)
        message("-- Physics engine debugging enabled")
        add_definitions(-DENABLE_PHYSICS_DEBUG=1)
    endif ()
endif ()

# Enable time measurement
if (ENABLE_MEASUREMENT)
    message("-- Time measurement enabled")
    add_definitions(-DENABLE_MEASUREMENT=1)
endif ()

# Build unit tests
if (BUILD_TESTS)
    message("-- Build unit tests")
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
endif ()

# ######################################################################### #

# Core libraries
add_subdirectory(core)
add_subdirectory(parser)
add_subdirectory(parser-xml)

# Add render library
if (ENABLE_RENDER)
    add_subdirectory(render)
endif ()

# Main simulator library
add_subdirectory(simulator)

# Add plugins that should be built
foreach (PLUGIN_NAME ${PLUGINS_BUILD})
    add_subdirectory(${DIR_PLUGINS}/${PLUGIN_NAME})
endforeach ()

# Build GUI project
if (ENABLE_GUI)
    add_subdirectory(gui)
endif ()

# Build CLI application
if (ENABLE_CLI)
    add_subdirectory(cli)
endif ()

# Build additional tools
if (BUILD_TOOLS)
    add_subdirectory(tools/shader-convert)
endif()

# ######################################################################### #

# Setup packages creation
include(Package)

# ######################################################################### #
