# ######################################################################### #

project(module-diffusion)

# Create library
add_library(module-diffusion
    Signal.hpp
    Module.hpp
    Module.cpp
    Generator.hpp
    Generator.cpp
    SignalGrid.hpp
    SignalGrid.cpp
)

# Setup dependencies
add_dependencies(module-diffusion
    core
    simulator
)

# ######################################################################### #
