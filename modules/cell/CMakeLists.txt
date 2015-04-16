# ######################################################################### #

project(module-cell)

# Create library
add_library(module-cell SHARED
    Library.hpp
    Library.cpp
    Module.hpp
    Module.cpp
    Cell.hpp
    Cell.cpp
    Yeast.hpp
    Yeast.cpp
    Generator.hpp
    Generator.cpp
)

# Setup dependencies
add_dependencies(module-cell
    core
    simulator
)

# Link libraries
target_link_libraries(module-cell
    simulator
)

# ######################################################################### #
