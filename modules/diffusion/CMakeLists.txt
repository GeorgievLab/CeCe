# ######################################################################### #

project(module-diffusion)

# Create library
add_library(module-diffusion
    Signal.hpp
    Module.hpp
    Module.cpp
    Generator.hpp
    Generator.cpp
    GeneratorCell.hpp
    GeneratorCell.cpp
    SignalGrid.hpp
    SignalGridDrawable.hpp
    SignalGridDrawable.cpp
)

# Setup dependencies
add_dependencies(module-diffusion
    core
    simulator
)

# ######################################################################### #
