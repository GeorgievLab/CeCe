# ######################################################################### #

project(cli)

if (ENABLE_RENDER)
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})

    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})

    set(RENDER_LIBS ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
    set(RENDER_DEP render)
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
    core
    ${RENDER_DEP}
    ${RENDER_LIBS}
    ${JPEG_LIBRARIES}
    -pthread
)

# Setup dependencies
add_dependencies(cli
    parser
    simulator
    core
    ${RENDER_DEP}
)

# ######################################################################### #
