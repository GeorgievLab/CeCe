# ######################################################################### #

# Project name
set(PROJECT_NAME parser-xml)

# Create project
project(${PROJECT_NAME} CXX)

# Create library
add_library(${PROJECT_NAME}
    SimulationFactory.hpp
    SimulationFactory.cpp
    ImmutableConfiguration.hpp
    MutableConfiguration.hpp

    # PugiXML library
    pugixml/pugixml.hpp
    pugixml/pugixml.cpp
)

# Setup dependencies
add_dependencies(${PROJECT_NAME}
    core
    parser
)

# ######################################################################### #
