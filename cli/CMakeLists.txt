# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Project
project(cli CXX)

if (ENABLE_RENDER)
    find_package(GLFW REQUIRED)
    include_directories(${GLFW_INCLUDE_DIR})

    set(RENDER_LIBS ${GLFW_LIBRARIES})

    if (ENABLE_BUILTIN_PHYSICS_DEBUG)
        find_package(OpenGL)
        include_directories(${OPENGL_INCLUDE_DIR})

        set(RENDER_LIBS ${RENDER_LIBS} ${OPENGL_LIBRARIES})
    endif ()
endif ()

set(SRCS
    main.cpp
)

if (ENABLE_BUILTIN_PHYSICS_DEBUG)
    set(SRCS ${SRCS}
        Render.hpp
        Render.cpp
    )
endif ()

# Create program
add_executable(${PROJECT_NAME} ${SRCS})

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_NAME}"
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    simulator
    parser-xml
    ${RENDER_LIBS}
)

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

# Setup dependencies
add_dependencies(${PROJECT_NAME}
    simulator
)

# Install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR_RUNTIME}
)

# ######################################################################### #
