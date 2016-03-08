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

##
## Build plugin helper function
##
## param: NAME      Plugin name.
## param: SOURCES   List of sources.
## param: LIBRARIES A list of required libraries.
## param: PLUGINS_REQUIRED A list of required plugins.
##
## Function uses PLUGINS_BUILTIN_${NAME} variable.
##
function(build_plugin NAME)

    # Get project name
    plugin_project_name(${NAME} FULLNAME)

    include(CMakeParseArguments)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES;DEPENDENCIES;PLUGINS_REQUIRED" ${ARGN})

    # Modify list of plugins (add prefixes)
    set(PLUGINS_DEPENDENCIES "")

    foreach (PLUGIN ${ARG_PLUGINS_REQUIRED})
        plugin_project_name(${PLUGIN} PLUGIN_NAME)
        list(APPEND PLUGINS_DEPENDENCIES "${PLUGIN_NAME}")
    endforeach ()

    # Create project
    project(${FULLNAME} CXX)

    # If plugin will be build-in
    if (PLUGINS_BUILTIN_${NAME})
        # Pass macro to inform about build-in version
        # TODO: change to plugin
        add_definitions(-DPLUGIN_BUILTIN=1)

        # Static library
        add_library(${PROJECT_NAME} ${ARG_SOURCES})

        # Setup dependencies
        if (ARG_DEPENDENCIES)
            add_dependencies(${PROJECT_NAME}
                cece
                ${ARG_DEPENDENCIES}
            )
        endif ()

        if (PLUGINS_DEPENDENCIES)
            add_dependencies(${PROJECT_NAME}
                ${PLUGINS_DEPENDENCIES}
            )
        endif ()

        # Link libraries
        target_link_libraries(${PROJECT_NAME}
            ${ARG_LIBRARIES}
        )

        # Required for Linux
        if (UNIX AND NOT APPLE)
            set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
        endif ()
    else ()
        # Shared module
        add_library(${PROJECT_NAME} SHARED ${ARG_SOURCES})

        # Setup dependencies
        add_dependencies(${PROJECT_NAME}
            cece
            ${ARG_DEPENDENCIES}
            ${PLUGINS_DEPENDENCIES}
        )

        # Link libraries
        target_link_libraries(${PROJECT_NAME}
            ${PLUGINS_DEPENDENCIES}
            ${ARG_LIBRARIES}
            cece
        )

        # Allow to link other plugins
        if (UNIX AND NOT APPLE)
            set_target_properties(${PROJECT_NAME} PROPERTIES
                INSTALL_RPATH "$ORIGIN"
            )
        elseif (APPLE)
            set_target_properties(${PROJECT_NAME} PROPERTIES
                MACOSX_RPATH On
            )
        endif ()

        # Create symbolic link
        if (DEV_PLUGINS)
            if (UNIX)
                add_custom_command(
                    TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E
                        create_symlink $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/${DIR_PLUGINS}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
                )
            elseif (WIN32)
                # Permission issue
                #add_custom_command(
                #    TARGET ${PROJECT_NAME} POST_BUILD
                #    COMMAND mklink ${PLUGINS_DIRECTORY}/$<TARGET_FILE_NAME:${PROJECT_NAME}> $<TARGET_FILE:${PROJECT_NAME}>
                #)
            endif ()
        endif ()

        # Install shared library
        install(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION ${INSTALL_DIR_PLUGINS}
            LIBRARY DESTINATION ${INSTALL_DIR_PLUGINS}
            COMPONENT Plugins
        )

        # Install header files
        install(DIRECTORY .
            DESTINATION ${INSTALL_DIR_INCLUDE}/${CMAKE_PROJECT_NAME}/plugins/${NAME}
            COMPONENT Headers
            FILES_MATCHING PATTERN "*.hpp"
        )

        if (APPLE)
            # Fix library paths
            install(
                CODE "
                    execute_process(COMMAND \"${CMAKE_SOURCE_DIR}/resources/scripts/macosx-libraries.sh\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_PLUGINS}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}\")
                    "
                COMPONENT Runtime
            )
        endif ()

        #set(CPACK_COMPONENT_${NAME}_GROUP "Plugins")
    endif ()
endfunction()

# ######################################################################### #

# Get plugin project name.
function(plugin_project_name PLUGIN_NAME PROJECT_NAME)
    set(${PROJECT_NAME} "plugin-${PLUGIN_NAME}" PARENT_SCOPE)
endfunction ()

# ######################################################################### #

# Get plugins project names.
function(plugin_project_names PLUGIN_NAMES PROJECT_NAMES)

    set(NAMES "")

    foreach (PLUGIN_NAME ${PLUGIN_NAMES})
        plugin_project_name(${PLUGIN_NAME} PROJECT_NAME)
        list(APPEND NAMES ${PROJECT_NAME})
    endforeach ()

    set(${PROJECT_NAMES} ${NAMES} PARENT_SCOPE)
endfunction ()

# ######################################################################### #

# Build tests
function(build_test PROJECT_NAME)

    # Only if tests are enabled
    if (DEV_TESTS_BUILD)

        # Parse arguments
        include(CMakeParseArguments)
        cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES;DEPENDENCIES;PLUGINS_REQUIRED" ${ARGN})

        # Modify list of plugins (add prefixes)
        set(PLUGINS_DEPENDENCIES "")

        foreach (PLUGIN ${ARG_PLUGINS_REQUIRED})
            plugin_project_name(${PLUGIN} PLUGIN_NAME)
            list(APPEND PLUGINS_DEPENDENCIES "${PLUGIN_NAME}")
        endforeach ()

        # Test name
        set(TEST_NAME "${PROJECT_NAME}_test")

        # Create executable
        add_executable(${TEST_NAME}
            ${ARG_SOURCES}
        )

        # Link GTest libraries
        target_link_libraries(${TEST_NAME}
            ${ARG_LIBRARIES}
            ${PLUGINS_DEPENDENCIES}
            ${GTEST_BOTH_LIBRARIES}
        )

        if (UNIX AND NOT APPLE)
            target_link_libraries(${TEST_NAME}
                -pthread
            )
        endif ()

        # Add test
        add_test(${PROJECT_NAME} ${TEST_NAME})
    endif ()
endfunction ()

# ######################################################################### #
