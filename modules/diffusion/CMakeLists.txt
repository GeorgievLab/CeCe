# ######################################################################### #

project(module-diffusion)

# Create library
add_library(module-diffusion
    DiffusionModule.hpp
    DiffusionModule.cpp
)

# Setup dependencies
add_dependencies(module-diffusion
    core
    simulator
)

# ######################################################################### #
