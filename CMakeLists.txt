# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# ######################################################################### #

# Add path to additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

# ######################################################################### #

# Workspace name
project(cell-sim)

# ######################################################################### #

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
set(DIR_PLUGINS "plugins")

# ######################################################################### #

# Options
option(ENABLE_GUI                   "Enable GUI" On)
option(ENABLE_CLI                   "Enable CLI" Off)
option(ENABLE_RENDER                "Enable simulation rendering" On)
option(ENABLE_SSE                   "Enable SSE instructions" On)
option(ENABLE_BUILTIN_PHYSICS       "Enable builtin physics engine (Box2D)" On)
option(ENABLE_MEASUREMENT           "Enable or disable time measurement" Off)
option(ENABLE_BUILTIN_PHYSICS_DEBUG "Enable or disable physics engine debugging" Off)

option(BUILD_TESTS                  "Build simulator part tests (requires GTest)" Off)
option(BUILD_TOOLS                  "Build support tools" Off)

option(DEV_PLUGINS                  "Enable special mode for developers that helps develop plugins (doesn't require installation)" Off)

# ######################################################################### #

# List of plugins values
set(PLUGINS_AVAILABLE "")
set(PLUGINS_BUILD "")
set(PLUGINS_BUILTIN "")

# Get list of available plugins
file(GLOB PLUGINS_DIRECTORIES RELATIVE "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}" "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}/*")
foreach(PLUGIN_DIR ${PLUGINS_DIRECTORIES})
    if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}/${PLUGIN_DIR}")
        list(APPEND PLUGINS_AVAILABLE ${PLUGIN_DIR})
        option(PLUGINS_BUILD_${PLUGIN_DIR} "Build plugin '${PLUGIN_DIR}'" On)
        option(PLUGINS_BUILTIN_${PLUGIN_DIR} "Build plugin '${PLUGIN_DIR}' as build-in plugin" Off)

        if (PLUGINS_BUILTIN_${PLUGIN_DIR})
            list(APPEND PLUGINS_BUILTIN ${PLUGIN_DIR})
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

# Install directories
set(INSTALL_PREFIX "/usr") # CPack with DEB ignores CMAKE_INSTALL_PREFIX
set(INSTALL_DIR_RUNTIME "bin")
set(INSTALL_DIR_LIBRARY "lib")
set(INSTALL_DIR_PLUGINS "${INSTALL_DIR_LIBRARY}/${CMAKE_PROJECT_NAME}")
set(INSTALL_DIR_PLUGINS_FULL "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_PLUGINS}")
set(INSTALL_DIR_FRAMEWORK "")
set(INSTALL_DIR_BUNDLE ".")
set(INSTALL_DIR_DATA ".")

# Linux installation directories
if (UNIX AND NOT APPLE)
    include(GNUInstallDirs)
    set(INSTALL_DIR_RUNTIME ${CMAKE_INSTALL_BINDIR})
    set(INSTALL_DIR_LIBRARY ${CMAKE_INSTALL_LIBDIR})
    set(INSTALL_DIR_PLUGINS "${INSTALL_DIR_LIBRARY}/${CMAKE_PROJECT_NAME}/${DIR_PLUGINS}")
    set(INSTALL_DIR_DATA "${CMAKE_INSTALL_DATAROOTDIR}/${CMAKE_PROJECT_NAME}")
endif ()

message(STATUS "Runtime directory: ${INSTALL_DIR_RUNTIME}")
message(STATUS "Library directory: ${INSTALL_DIR_LIBRARY}")
message(STATUS "Framework directory: ${INSTALL_DIR_FRAMEWORK}")
message(STATUS "Bundle directory: ${INSTALL_DIR_BUNDLE}")
message(STATUS "Data directory: ${INSTALL_DIR_DATA}")

# Default plugins directory
set(PLUGINS_DIRECTORY "${INSTALL_PREFIX}/${INSTALL_DIR_PLUGINS}")

if (DEV_PLUGINS)
    # Directory is in build directory
    set(PLUGINS_DIRECTORY "${PROJECT_BINARY_DIR}/${DIR_PLUGINS}")
endif ()

# ######################################################################### #

# Default include directories
include_directories(${CMAKE_SOURCE_DIR})

# Boost libraries
set(Boost_USE_STATIC_LIBS On)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# ######################################################################### #

# GUI enabled
if (ENABLE_GUI)
    message(STATUS "Build GUI application")
    set(ENABLE_RENDER 1)
else ()
    # If GUI is disabled build CLI
    set(ENABLE_CLI 1)
endif ()

# CLI enabled
if (ENABLE_CLI)
    message(STATUS "Build CLI application")
endif ()

# Rendering enabled
if (ENABLE_RENDER)
    message(STATUS "OpenGL rendering support")
    add_definitions(-DENABLE_RENDER=1)
endif ()

if (ENABLE_SSE)
    # TODO: detect SSE support
    message(STATUS "Use SSE instructions")
    add_definitions(-DENABLE_SSE=1)
endif ()

# Build-in physics enabled
if (ENABLE_BUILTIN_PHYSICS)
    message(STATUS "Using build-in physics (Box2D)")
    find_package(BOX2D REQUIRED)
    include_directories(${BOX2D_INCLUDE_DIR})

    add_definitions(-DENABLE_PHYSICS=1)

    if (ENABLE_BUILTIN_PHYSICS_DEBUG)
        message(STATUS "Physics engine debugging enabled")
        add_definitions(-DENABLE_PHYSICS_DEBUG=1)
    endif ()
endif ()

# Enable time measurement
if (ENABLE_MEASUREMENT)
    message(STATUS "Time measurement enabled")
    add_definitions(-DENABLE_MEASUREMENT=1)
endif ()

# Build unit tests
if (BUILD_TESTS)
    message(STATUS "Build unit tests")
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

# Install examples
install(DIRECTORY "${CMAKE_SOURCE_DIR}/examples"
    DESTINATION "${INSTALL_DIR_DATA}"
    FILES_MATCHING PATTERN "*.xml"
)

# ######################################################################### #

# Setup packages creation
include(Package)

# ######################################################################### #
