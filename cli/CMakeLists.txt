# ######################################################################### #

project(cli)

if (ENABLE_RENDER)
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})

    set(RENDER_LIBS ${GLUT_LIBRARIES})
endif ()

# Create program
add_executable(cli
    main.cpp
)

# Link libraries
target_link_libraries(cli
    module-diffusion-streamlines
    module-diffusion
    module-streamlines
    module-physics
    module-cell
    parser
    simulator
    ${RENDER_LIBS}
    -pthread
)

# Setup dependencies
add_dependencies(cli
    parser
    simulator
)

# ######################################################################### #
