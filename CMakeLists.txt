# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #
#                                                                           #
# This file is part of CeCe.                                                #
#                                                                           #
# CeCe is free software: you can redistribute it and/or modify              #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation, either version 3 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# CeCe is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with CeCe.  If not, see <http://www.gnu.org/licenses/>.             #
#                                                                           #
# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# ######################################################################### #

# Add path to additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

# ######################################################################### #

# Workspace name
project(cece)

# ######################################################################### #

# Application name
set(APP_NAME "CeCe")

# Application version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 3)
set(VERSION_SPECIAL "")

# Full version string
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Directory with plugins
set(DIR_PLUGINS "${CMAKE_PROJECT_NAME}/plugins")

# ######################################################################### #

# Options
option(CONFIG_CLI_ENABLE            "Build CLI application" On)
option(CONFIG_GUI_ENABLE            "Build GUI application" Off)
option(CONFIG_RENDER_ENABLE         "Enable simulation rendering (vizualization support)" On)
option(CONFIG_PHYSICS_BUILTIN       "Enable builtin physics engine (Box2D)" On)

# Real type
set(CONFIG_REAL_TYPE "double" CACHE STRING "Type used for real values")
set_property(CACHE CONFIG_REAL_TYPE PROPERTY STRINGS "float" "double" "long double")

# Dev options
option(DEV_TESTS_BUILD              "Build simulator part tests (requires GTest)" Off)
option(DEV_TESTS_EXAMPLES           "Enable examples testing" Off)
option(DEV_PLUGINS                  "Enable special mode for developers that helps develop plugins (doesn't require installation)" Off)
option(DEV_PHYSICS_BUILTIN_DEBUG    "Enable or disable physics engine debugging" Off)
option(DEV_PERFORMANCE_MEASUREMENT  "Enable or disable time measurement" Off)
option(DEV_BUILD_TOOLS              "Build additional tools used for developing" Off)

if (APPLE)
    set(MACOSX_VERSION_MIN "10.9" CACHE STRING "Minimum version of MacOS X to support")
    set(OSX_LIB_DIRS "" CACHE STRING "Additional directories for libraries packaged in Bundle")
endif ()

# If simulator should be thread safe
set(THREAD_SAFE Off)

# Must be defined
if (NOT CONFIG_REAL_TYPE)
    message(FATAL_ERROR "Missing 'CONFIG_REAL_TYPE' option")
endif ()

# ######################################################################### #

# List of plugins variables
set(PLUGINS_AVAILABLE "")
set(PLUGINS_BUILD "")
set(PLUGINS_BUILTIN "")

# Get list of available plugins
file(GLOB PLUGINS_DIRECTORIES RELATIVE "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}" "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}/*")
foreach(PLUGIN_DIR ${PLUGINS_DIRECTORIES})
    if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${DIR_PLUGINS}/${PLUGIN_DIR}")
        list(APPEND PLUGINS_AVAILABLE ${PLUGIN_DIR})
        option(PLUGINS_BUILD_${PLUGIN_DIR} "Build plugin '${PLUGIN_DIR}'" On)
        option(PLUGINS_BUILTIN_${PLUGIN_DIR} "Build plugin '${PLUGIN_DIR}' as builtin" Off)

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
set(INSTALL_DIR_RUNTIME "bin")
set(INSTALL_DIR_LIBRARY "lib")
set(INSTALL_DIR_INCLUDE "include")
set(INSTALL_DIR_PLUGINS "${INSTALL_DIR_LIBRARY}/${CMAKE_PROJECT_NAME}")
set(INSTALL_DIR_PLUGINS_FULL "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_PLUGINS}")
set(INSTALL_DIR_FRAMEWORK "")
set(INSTALL_DIR_BUNDLE ".")
set(INSTALL_DIR_DATA ".")
set(INSTALL_DIR_MANUAL ".")

# Linux installation directories
if (UNIX AND NOT APPLE)
    #set(INSTALL_PREFIX "/usr") # CPack with DEB ignores CMAKE_INSTALL_PREFIX
    include(GNUInstallDirs)
    set(INSTALL_DIR_RUNTIME "${CMAKE_INSTALL_BINDIR}")
    set(INSTALL_DIR_LIBRARY "${CMAKE_INSTALL_LIBDIR}")
    set(INSTALL_DIR_INCLUDE "${CMAKE_INSTALL_INCLUDEDIR}")
    set(INSTALL_DIR_PLUGINS "${INSTALL_DIR_LIBRARY}/${DIR_PLUGINS}")
    set(INSTALL_DIR_DATA    "${CMAKE_INSTALL_DATAROOTDIR}/${CMAKE_PROJECT_NAME}")
    set(INSTALL_DIR_MANUAL  "${CMAKE_INSTALL_DOCDIR}")
elseif (APPLE)
    set(INSTALL_DIR_RUNTIME "bin")
    set(INSTALL_DIR_LIBRARY "lib")
    set(INSTALL_DIR_INCLUDE "include")
    set(INSTALL_DIR_PLUGINS "plugins")
    set(INSTALL_DIR_BUNDLE  "bin")
    set(INSTALL_DIR_MANUAL  "manual")
elseif (WIN32)
    set(INSTALL_DIR_RUNTIME ".")
    set(INSTALL_DIR_LIBRARY ".")
    set(INSTALL_DIR_INCLUDE "include")
    set(INSTALL_DIR_PLUGINS ".")
    set(INSTALL_DIR_MANUAL  "manual")
    set(PLUGINS_DIRECTORY ".")
endif ()

message(STATUS "Runtime directory: ${INSTALL_DIR_RUNTIME}")
message(STATUS "Library directory: ${INSTALL_DIR_LIBRARY}")
message(STATUS "Framework directory: ${INSTALL_DIR_FRAMEWORK}")
message(STATUS "Bundle directory: ${INSTALL_DIR_BUNDLE}")
message(STATUS "Data directory: ${INSTALL_DIR_DATA}")
message(STATUS "Manual directory: ${INSTALL_DIR_MANUAL}")

# ######################################################################### #

# Default include directories
include_directories(${CMAKE_SOURCE_DIR})

# Boost libraries
set(Boost_USE_STATIC_LIBS On)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# ######################################################################### #

# Directory for configuration file
include_directories(${CMAKE_BINARY_DIR})

# ######################################################################### #

if (MINGW)
    # Fix for MinGW bug
    # http://stackoverflow.com/questions/33749413/error-hypot-was-not-declared-in-this-scope
    add_definitions(-D__NO_INLINE__)
endif ()

if (APPLE)
    add_definitions(-mmacosx-version-min=${MACOSX_VERSION_MIN})
endif()

# GUI enabled
if (CONFIG_GUI_ENABLE)
    message(STATUS "Build GUI application")
    set(CONFIG_RENDER_ENABLE On)
    set(THREAD_SAFE On)
else ()
    # If GUI is disabled build CLI
    set(CONFIG_CLI_ENABLE On)
endif ()

# CLI enabled
if (CONFIG_CLI_ENABLE)
    message(STATUS "Build CLI application")
endif ()

# Rendering enabled
if (CONFIG_RENDER_ENABLE)
    message(STATUS "OpenGL rendering support")
endif ()

# Build-in physics enabled
if (CONFIG_PHYSICS_BUILTIN)
    message(STATUS "Using build-in physics (Box2D)")
    find_package(BOX2D REQUIRED)
    include_directories(${BOX2D_INCLUDE_DIR})

    if (DEV_PHYSICS_BUILTIN_DEBUG)
        message(STATUS "Physics engine debugging enabled")
    endif ()
endif ()

# Enable time measurement
if (DEV_PERFORMANCE_MEASUREMENT)
    message(STATUS "Time measurement enabled")
    add_definitions(-DENABLE_MEASUREMENT=1)
endif ()

# Build unit tests
if (DEV_TESTS_BUILD)
    message(STATUS "Build unit tests")
    enable_testing()
    include(GTest)
endif ()

# Set real type
message(STATUS "Real type: ${CONFIG_REAL_TYPE}")

# ######################################################################### #

# CeCe simulator
add_subdirectory(cece)

# Add plugins that should be built
foreach (PLUGIN_NAME ${PLUGINS_BUILD})
    add_subdirectory(${DIR_PLUGINS}/${PLUGIN_NAME})
endforeach ()

# Build GUI project
if (CONFIG_GUI_ENABLE)
    add_subdirectory(app/gui)
endif ()

# Build CLI application
if (CONFIG_CLI_ENABLE)
    add_subdirectory(app/cli)
endif ()

# Build additional tools
if (DEV_BUILD_TOOLS)
    add_subdirectory(tools/shader-convert)
    add_subdirectory(tools/format)
endif()

# ######################################################################### #

# Install examples
install(DIRECTORY "${CMAKE_SOURCE_DIR}/examples"
    COMPONENT Examples
    DESTINATION "${INSTALL_DIR_DATA}"
    FILES_MATCHING PATTERN "*"
)

install(FILES "${CMAKE_SOURCE_DIR}/Copyright.txt"
    DESTINATION ${INSTALL_DIR_DATA}
)

# ######################################################################### #

# Add testing by examples
include(ExampleTests)

# ######################################################################### #

# Setup packages creation
include(Package)

# ######################################################################### #

# Generate manual
include(Manual)

# ######################################################################### #

