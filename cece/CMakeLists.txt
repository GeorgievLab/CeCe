# ######################################################################### #
# Georgiev Lab (c) 2015-2016                                                #
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

cmake_minimum_required(VERSION 2.8.8)

# ######################################################################### #

# CeCe shared library
project(cece CXX)

# ######################################################################### #

# Include build-in libraries
set(PLUGINS_LIBS "")
set(BUILTIN_PLUGINS "")

# Foreach build-in plugins
foreach (PLUGIN ${PLUGINS_BUILTIN})
    message(STATUS "Builtin plugin: ${PLUGIN}")
    plugin_project_name(${PLUGIN} PLUGIN_PROJECT)
    set(PLUGINS_LIBS ${PLUGINS_LIBS} ${PLUGIN_PROJECT})
    # Fix module name: replace '-' by '_'
    string(REPLACE "-" "_" FIXED_PLUGIN ${PLUGIN})
    set(BUILTIN_PLUGINS "${BUILTIN_PLUGINS}ITEM(${PLUGIN},${FIXED_PLUGIN})")
endforeach ()

# ######################################################################### #

# Include Python API
if (UNIX AND NOT APPLE)
    # Link boost dynamically on Linux
    set(Boost_USE_STATIC_LIBS Off)
endif ()

find_package(Boost REQUIRED filesystem system)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})

# ######################################################################### #

# Include OpenGL if rendering is enabled
if (CONFIG_RENDER_ENABLE)
    # OpenGL library is required
    find_package(OpenGL REQUIRED)

    # Include OpenGL path
    include_directories(${OPENGL_INCLUDE_DIR})
endif ()

# ######################################################################### #

# Convert configuration names
set(CECE_ENABLE_RENDER ${CONFIG_RENDER_ENABLE})
set(CECE_ENABLE_BOX2D_PHYSICS ${CONFIG_PHYSICS_BUILTIN})
set(CECE_ENABLE_BOX2D_PHYSICS_DEBUG ${DEV_PHYSICS_BUILTIN_DEBUG})
set(CECE_THREAD_SAFE ${THREAD_SAFE})

# Create config file
configure_file(config.hpp.in config.hpp)

# ######################################################################### #

# Core files
add_subdirectory(core)
add_subdirectory(config)
add_subdirectory(loader)
add_subdirectory(init)
add_subdirectory(module)
add_subdirectory(object)
add_subdirectory(program)
add_subdirectory(plugin)

# Renderer files
if (CONFIG_RENDER_ENABLE)
    add_subdirectory(render)
endif ()

# Simulation files
add_subdirectory(simulator)

# ######################################################################### #

# Sources
set(SRCS
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:config>
    $<TARGET_OBJECTS:loader>
    $<TARGET_OBJECTS:init>
    $<TARGET_OBJECTS:module>
    $<TARGET_OBJECTS:object>
    $<TARGET_OBJECTS:program>
    $<TARGET_OBJECTS:plugin>
    $<TARGET_OBJECTS:simulator>
)

if (CONFIG_RENDER_ENABLE)
    set(SRCS ${SRCS} $<TARGET_OBJECTS:render>)
endif ()

# Create library
add_library(${PROJECT_NAME} SHARED ${SRCS})

# ######################################################################### #

# Generate export header (MSVC)
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
    EXPORT_FILE_NAME "export.hpp"
)

# Set framework flag
#set_target_properties(${PROJECT_NAME} PROPERTIES FRAMEWORK 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH On)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${PLUGINS_LIBS}
    ${Boost_LIBRARIES}
)

if (CONFIG_RENDER_ENABLE)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif ()

if (CONFIG_PHYSICS_BUILTIN)
    # Link libraries
    target_link_libraries(${PROJECT_NAME} ${BOX2D_LIBRARIES})
endif ()

if (MINGW)
    target_link_libraries(${PROJECT_NAME} Ws2_32)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} dl)
endif ()

# ######################################################################### #

# Install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR_LIBRARY}
    LIBRARY DESTINATION ${INSTALL_DIR_LIBRARY}
    COMPONENT Library
)

# Install configuration file
install(FILES
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/config.hpp
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/export.hpp
    DESTINATION ${INSTALL_DIR_INCLUDE}/${CMAKE_PROJECT_NAME}
    COMPONENT Headers
)

if (APPLE)
    # Fix library paths
    install(
        CODE "
            execute_process(COMMAND \"${CMAKE_SOURCE_DIR}/resources/scripts/macosx-libraries.sh\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_LIBRARY}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}\")
            "
        COMPONENT Runtime
    )
endif ()

if (APPLE OR WIN32)
    get_filename_component(LIBRARY_PATH ${BOX2D_LIBRARIES} REALPATH)

    # Install Box2D library
    install(FILES
        ${LIBRARY_PATH}
        DESTINATION ${INSTALL_DIR_LIBRARY}
    )
endif ()

# ######################################################################### #
