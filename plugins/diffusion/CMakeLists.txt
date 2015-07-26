# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Sources
set(SRCS
    Library.cpp
    Module.hpp
    Module.cpp
    Generator.hpp
    Generator.cpp
    StoreState.hpp
    StoreState.cpp
)

# ######################################################################### #

# Build plugin
build_plugin(diffusion
    SOURCES ${SRCS}
)

# ######################################################################### #
