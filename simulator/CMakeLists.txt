# ######################################################################### #

project(simulator)

# Create library
add_library(simulator
    Cell.hpp
    Cell.cpp
    World.hpp
    World.cpp
    WorldFactory.hpp
    WorldFactory.cpp
    Simulator.hpp
    Simulator.cpp
    Barrier.hpp
    Barrier.cpp
    Yeast.hpp
    Yeast.cpp
)

# Setup dependencies
add_dependencies(simulator
    core
    physics
)

# ######################################################################### #
