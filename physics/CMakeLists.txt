# ######################################################################### #

project(physics)

# Bullet physics
find_package(Bullet REQUIRED)

# Include dirs
include_directories(${BULLET_INCLUDE_DIRS})

# Create library
add_library(physics
    Object.hpp
    Object.cpp
    World.hpp
    World.cpp
    Ground.hpp
    Ground.cpp
    Cell.hpp
    Cell.cpp
    Barrier.hpp
    Barrier.cpp
)

target_link_libraries(physics
    ${BULLET_LIBRARIES}
)

# Setup dependencies
add_dependencies(physics
    core
)

# ######################################################################### #
