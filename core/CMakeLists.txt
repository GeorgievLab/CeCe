# ######################################################################### #

project(core)

# Create library
add_library(core
    constants.hpp
    Matrix.hpp
    Units.hpp
    Vector.hpp
    Vector.cpp
    AlignedAllocator.hpp
    AlignedAllocator.cpp
)

# ######################################################################### #
