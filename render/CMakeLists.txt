# ######################################################################### #

project(render)

# Sources
set(CXX_SRCS
    Yeast.cpp
)

# Create library
add_library(render
    ${CXX_SRCS}
)

# Setup dependencies
add_dependencies(render core)

# ######################################################################### #
