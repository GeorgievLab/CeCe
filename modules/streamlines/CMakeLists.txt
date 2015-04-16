# ######################################################################### #

project(module-streamlines)

# Create library
add_library(module-streamlines SHARED
    Module.hpp
    Module.cpp
    Library.hpp
    Library.cpp
)

# Setup dependencies
add_dependencies(module-streamlines
    core
    simulator
)

# Link libraries
target_link_libraries(module-streamlines
    simulator
)

# ######################################################################### #
