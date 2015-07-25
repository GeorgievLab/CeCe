# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Create project
project(loader-reactions CXX)

# ######################################################################### #

# Create library
add_library(${PROJECT_NAME}
    SimulationLoader.hpp
    SimulationLoader.cpp
)

# ######################################################################### #

# Setup dependencies
add_dependencies(${PROJECT_NAME}
    simulator
)

# ######################################################################### #
