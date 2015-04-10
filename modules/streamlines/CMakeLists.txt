# ######################################################################### #

project(module-streamlines)

# Create library
add_library(module-streamlines
    StreamlinesModule.hpp
    StreamlinesModule.cpp
)

# Setup dependencies
add_dependencies(module-streamlines
    core
    simulator
)

# ######################################################################### #
