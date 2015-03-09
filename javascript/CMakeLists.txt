# ######################################################################### #

project(javascript)

# Add sources
add_library(javascript
    WorldFactory.h
    WorldFactory.cpp
    World.h
    World.cpp
    Cell.hpp
    Cell.cpp
    Barrier.hpp
    Barrier.cpp
    Yeast.hpp
    Yeast.cpp
)

# Library dependency
target_link_libraries(javascript
    v8
)

# Setup dependencies
add_dependencies(simulator
    core
    physics
)

# ######################################################################### #
