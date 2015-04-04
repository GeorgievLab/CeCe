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
    World.hpp
    WorldFactory.hpp
    Simulator.hpp
    Grid.hpp
    Module.hpp
    StreamlinesModule.hpp
    DiffusionModule.hpp
    DiffusionStreamlinesModule.hpp

    # Sources
    Object.cpp
    DynamicObject.cpp
    Cell.cpp
    #Yeast.cpp
    World.cpp
    WorldFactory.cpp
    Simulator.cpp
    StreamlinesModule.cpp
    DiffusionModule.cpp
    DiffusionStreamlinesModule.cpp
)

# Setup dependencies
add_dependencies(simulator
    core
)

# ######################################################################### #
