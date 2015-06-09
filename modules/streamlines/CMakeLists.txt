# ######################################################################### #

# Project name
set(PROJECT_NAME module-streamlines)

# Create project
project(${PROJECT_NAME} CXX)

#find_package(ImageMagick REQUIRED COMPONENTS Magick++)
#include_directories(${ImageMagick_INCLUDE_DIRS})

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.hpp
    Library.cpp
    Lattice.hpp
    Lattice.cpp
    LatticeData.hpp
    LatticeData.cpp
    ObstacleMap.hpp
    ObstacleMap.cpp
)

add_definitions(-DOPT_DRAW_VELOCITY=1)

# Build static / dynamic
list(FIND MODULES_BUILDIN "streamlines" IS_STATIC)

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

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        #${ImageMagick_LIBRARIES}
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
        simulator
        #${ImageMagick_LIBRARIES}
    )

    # Install
    install(TARGETS ${PROJECT_NAME}
        DESTINATION lib
    )

endif ()

# ######################################################################### #
