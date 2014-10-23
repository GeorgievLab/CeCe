# ######################################################################### #

project(simulator)

# Sources
set(CXX_SRCS
    Cell.cpp
    World.cpp
    Yeast.cpp
    WorldFactory.cpp
    Simulator.cpp
)

# Create library
add_library(simulator
    ${CXX_SRCS}
)

# Setup dependencies
add_dependencies(simulator core physics render)

# ######################################################################### #
