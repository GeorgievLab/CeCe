# ######################################################################### #

project(simulator)

# Create library
add_library(simulator
    # Headers
    Position.hpp
    Velocity.hpp
    Object.hpp
    StaticObject.hpp
    DynamicObject.hpp
    Simulation.hpp
    SimulationFactory.hpp
    Simulator.hpp
    Grid.hpp
    Module.hpp

    # Sources
    Object.cpp
    DynamicObject.cpp
    Simulation.cpp
    SimulationFactory.cpp
    Simulator.cpp
)

# Setup dependencies
add_dependencies(simulator
    core
)

# ######################################################################### #
