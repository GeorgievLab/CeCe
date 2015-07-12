# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
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
        # Required for Linux
        if (UNIX AND NOT APPLE)
            add_compile_options(-fPIC)
        endif ()

        # Pass macro to inform about build-in version
        # TODO: change to plugin
        add_definitions(-DPLUGIN_BUILTIN=1)

        # Static library
        add_library(${PROJECT_NAME} ${ARG_SOURCES})

        # Setup dependencies
        add_dependencies(${PROJECT_NAME}
            core
            ${ARG_DEPENDENCIES}
            ${PLUGINS_DEPENDENCIES}
        )

        # Link libraries
        target_link_libraries(${PROJECT_NAME}
            ${ARG_LIBRARIES}
        )
    else ()
        # Shared module
        add_library(${PROJECT_NAME} SHARED ${ARG_SOURCES})

        # Setup dependencies
        add_dependencies(${PROJECT_NAME}
            core
            simulator
            ${ARG_DEPENDENCIES}
            ${PLUGINS_DEPENDENCIES}
        )

        # Link libraries
        target_link_libraries(${PROJECT_NAME}
            simulator
            ${ARG_LIBRARIES}
            ${PLUGINS_DEPENDENCIES}
        )

        # Create symbolic link
        if (DEV_PLUGINS)
            if (UNIX)
                add_custom_command(
                    TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E
                        create_symlink $<TARGET_FILE:${PROJECT_NAME}> ${PLUGINS_DIRECTORY}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
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
            DESTINATION ${INSTALL_DIR_PLUGINS}
            COMPONENT Plugins
        )

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
    if (BUILD_TESTS)

        # Parse arguments
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
