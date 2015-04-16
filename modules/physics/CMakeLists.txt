# ######################################################################### #

project(module-physics)

# Create library
add_library(module-physics SHARED
    Library.hpp
    Library.cpp
    Module.hpp
    Module.cpp
)

# Setup dependencies
add_dependencies(module-physics
    core
    simulator
)

# Link libraries
target_link_libraries(module-physics
    simulator
)

# ######################################################################### #
