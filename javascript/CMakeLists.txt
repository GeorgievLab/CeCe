# ######################################################################### #

project(javascript)

# Add sources
add_library(javascript
    WorldFactory.hpp
    WorldFactory.cpp
    World.hpp
    World.cpp
    Cell.hpp
    Cell.cpp
    Barrier.hpp
    Barrier.cpp
    Yeast.hpp
    Yeast.cpp
    Canvas.hpp
    Canvas.cpp
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
