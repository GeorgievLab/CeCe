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
    Cell.hpp
    #Yeast.hpp
    Simulation.hpp
    SimulationFactory.hpp
    Simulator.hpp
    Grid.hpp
    Module.hpp

    # Sources
    Object.cpp
    DynamicObject.cpp
    Cell.cpp
    #Yeast.cpp
    Simulation.cpp
    SimulationFactory.cpp
    Simulator.cpp
)

# Setup dependencies
add_dependencies(simulator
    core
)

# ######################################################################### #
