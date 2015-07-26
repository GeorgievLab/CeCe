# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

cmake_minimum_required(VERSION 2.8.8)

# ######################################################################### #

# Create project
project(loader-xml CXX)

# ######################################################################### #

# Create library
add_library(${PROJECT_NAME}
    SimulationLoader.hpp
    SimulationLoader.cpp
    Configuration.hpp

    # PugiXML library
    pugixml/pugixml.hpp
    pugixml/pugixml.cpp
)

# ######################################################################### #

# Setup dependencies
add_dependencies(${PROJECT_NAME}
    simulator
)

# ######################################################################### #
