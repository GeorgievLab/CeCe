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

set(MODULES_LIBS "")
set(MODULES_SHARED ${MODULES})

# Remove buildin
foreach (MODULE ${MODULES_BUILDIN})
    list(REMOVE_ITEM MODULES_SHARED ${MODULE})
endforeach ()

foreach (MODULE ${MODULES_SHARED})
    message("-- Shared module: ${MODULE}")
    set(MODULES_LIBS ${MODULES_LIBS} module-${MODULE})
endforeach ()

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "cell-sim-cli"
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${MODULES_LIBS}
    simulator
    parser-xml
    ${RENDER_LIBS}
    -pthread
)

# Setup dependencies
add_dependencies(${PROJECT_NAME}
    simulator
)

# Install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# ######################################################################### #
