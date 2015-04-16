# ######################################################################### #

project(simulator)

# Create library
add_library(simulator SHARED
    Position.hpp
    Velocity.hpp
    Object.hpp
    Object.cpp
    StaticObject.hpp
    DynamicObject.hpp
    DynamicObject.cpp
    Simulation.hpp
    Simulation.cpp
    SimulationFactory.hpp
    SimulationFactory.cpp
    Simulator.hpp
    Simulator.cpp
    Grid.hpp
    Module.hpp
    Library.hpp
    Library.cpp
)

# Use different name
#set_target_properties(simulator PROPERTIES
#    OUTPUT_NAME "sim"
#)

# Setup dependencies
add_dependencies(simulator
    core
    render
)

# ######################################################################### #
