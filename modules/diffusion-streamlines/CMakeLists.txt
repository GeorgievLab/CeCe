# ######################################################################### #

project(module-diffusion-streamlines)

# Create library
add_library(module-diffusion-streamlines
    DiffusionStreamlinesModule.hpp
    DiffusionStreamlinesModule.cpp
)

# Setup dependencies
add_dependencies(module-diffusion-streamlines
    core
    simulator
)

# ######################################################################### #
