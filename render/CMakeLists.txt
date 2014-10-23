# ######################################################################### #

project(render)

# Sources
set(CXX_SRCS
    Context.h
    Yeast.h
    Yeast.cpp
    World.h
    World.cpp
)

# Create library
add_library(render
    ${CXX_SRCS}
)

# Setup dependencies
add_dependencies(render core)

# ######################################################################### #
