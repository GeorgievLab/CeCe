# ######################################################################### #

# Project name
set(PROJECT_NAME module-diffusion-cylinder-streamlines)

# Create project
project(${PROJECT_NAME} CXX)

# Sources
set(SRCS
    Library.hpp
    Library.cpp
    Module.hpp
    Module.cpp
)

# Build static / dynamic
list(FIND MODULES_BUILDIN "diffusion-cylinder-streamlines" IS_STATIC)

if (${IS_STATIC} GREATER -1)
    message("-- Module ${PROJECT_NAME} is build-in")

    if (UNIX)
        add_compile_options(-fPIC)
    endif ()

    add_definitions(-DLIBRARY_BUILDIN=1)
    add_library(${PROJECT_NAME} ${SRCS})

    # Setup dependencies
    add_dependencies(${PROJECT_NAME}
        core
    )
else ()
    message("-- Module ${PROJECT_NAME} is dynamic")
    add_library(${PROJECT_NAME} SHARED ${SRCS})

    # Setup dependencies
    add_dependencies(${PROJECT_NAME}
        core
        simulator
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        diffusion
        cylinder-streamlines
        simulator
    )
endif ()

# ######################################################################### #
