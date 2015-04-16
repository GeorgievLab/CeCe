# ######################################################################### #

project(module-diffusion-streamlines)

# Create library
add_library(module-diffusion-streamlines SHARED
    Library.hpp
    Library.cpp
    Module.hpp
    Module.cpp
)

# Setup dependencies
add_dependencies(module-diffusion-streamlines
    core
    simulator
    module-streamlines
    module-diffusion
)

# Link libraries
target_link_libraries(module-diffusion-streamlines
    simulator
    module-streamlines
    module-diffusion
)

# ######################################################################### #
