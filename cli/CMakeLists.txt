# ######################################################################### #

project(cli)

if (ENABLE_RENDER)
    set(RENDER_LIBS GL GLU)
    set(RENDER_DEP render)
endif ()

# Create program
add_executable(cli
    main.cpp
)

# Link libraries
target_link_libraries(cli
    javascript
    simulator
    ${RENDER_DEP}
    physics
    v8
    BulletDynamics
    BulletCollision
    LinearMath
    ${RENDER_LIBS}
    ${JPEG_LIBRARIES}
)

# Setup dependencies
add_dependencies(cli
    javascript
    simulator
    core
    ${RENDER_DEP}
    physics
)

# ######################################################################### #
