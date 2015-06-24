# ######################################################################### #

# Project name
set(PROJECT_NAME cli)

project(${PROJECT_NAME} CXX)

if (ENABLE_RENDER)
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})

    set(RENDER_LIBS ${GLUT_LIBRARIES})

    if (ENABLE_PHYSICS_DEBUG)
        find_package(OpenGL)
        include_directories(${OPENGL_INCLUDE_DIR})

        set(RENDER_LIBS ${RENDER_LIBS} ${OPENGL_LIBRARIES})
    endif ()
endif ()

set(SRCS
    main.cpp
)

if (ENABLE_PHYSICS_DEBUG)
    set(SRCS ${SRCS}
        Render.hpp
        Render.cpp
    )
endif ()

# Create program
add_executable(${PROJECT_NAME} ${SRCS})

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "cell-sim-cli"
)

set(PLUGINS_TO_LINK "")

if (ENABLE_PLUGINS_DIRECT_LINK)
    plugin_project_names("${PLUGINS_BUILD}" PLUGINS_TO_LINK)
endif ()

# Link libraries
target_link_libraries(${PROJECT_NAME}
    simulator
    parser-xml
    ${PLUGINS_TO_LINK}
    ${RENDER_LIBS}
    -pthread
)

# Setup dependencies
add_dependencies(${PROJECT_NAME}
    simulator
)

# Install
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION ${INSTALL_DIR_BUNDLE}
    RUNTIME DESTINATION ${INSTALL_DIR_RUNTIME}
)

# ######################################################################### #
