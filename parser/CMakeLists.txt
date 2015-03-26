# ######################################################################### #

project(parser)

# Pugixml is external project
include(ExternalProject)

# Create library
add_library(parser
    WorldFactory.hpp
    Parser.hpp

    WorldFactory.cpp
    Parser.cpp

    pugixml/pugixml.hpp
    pugixml/pugixml.cpp
)

# Setup dependencies
add_dependencies(parser
    core
    simulator
)

# Link libraries
target_link_libraries(parser
    simulator
)

# ######################################################################### #
