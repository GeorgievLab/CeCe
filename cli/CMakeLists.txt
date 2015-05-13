# ######################################################################### #

project(cli)

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
add_executable(cli ${SRCS})

set(MODULES_LIBS "")

foreach (MODULE ${MODULES})
    set(MODULES_LIBS ${MODULES_LIBS} module-${MODULE})
endforeach ()

# Link libraries
target_link_libraries(cli
    ${MODULES_LIBS}
    simulator
    parser-xml
    ${RENDER_LIBS}
    -pthread
)

# Setup dependencies
add_dependencies(cli
    simulator
)

# ######################################################################### #
