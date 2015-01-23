# ######################################################################### #

project(simulator)

# Create library
add_library(simulator
    Cell.h
    Cell.cpp
    World.h
    World.cpp
    WorldFactory.h
    WorldFactory.cpp
    Simulator.h
    Simulator.cpp
)

# Setup dependencies
add_dependencies(simulator
    core
    physics
)

# ######################################################################### #
