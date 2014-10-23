# ######################################################################### #

project(physics)

# Bullet physics
find_package(Bullet REQUIRED)

# Include dirs
include_directories(${BULLET_INCLUDE_DIRS})

# Sources
set(CXX_SRCS
    Object.cpp
    World.cpp
    Ground.cpp
    Cell.cpp
)

# Create library
add_library(physics
    ${CXX_SRCS}
)

target_link_libraries(physics
    ${BULLET_LIBRARIES}
)

# Setup dependencies
add_dependencies(physics core)

# ######################################################################### #
