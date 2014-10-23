# ######################################################################### #

project(javascript)

# C++ source
set(CXX_SRCS
    WorldFactory.cpp
    World.cpp
)

# Add sources
add_library(javascript
    ${CXX_SRCS}
)

# Library dependency
target_link_libraries(javascript
    v8
)

# Setup dependencies
add_dependencies(simulator core physics)

# ######################################################################### #
