# ######################################################################### #

project(parser)

# Create library
add_library(parser
    Parser.hpp
    Parser.cpp
    SimulationFactory.hpp
    SimulationFactory.cpp

    # PugiXML library
    pugixml/pugixml.hpp
    pugixml/pugixml.cpp
)

# Setup dependencies
add_dependencies(parser
    core
    simulator
)

# ######################################################################### #
