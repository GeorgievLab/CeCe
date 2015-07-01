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
)

# Build plugin
build_plugin(diffusion-cylinder-streamlines
    SOURCES ${SRCS}
    PLUGINS_REQUIRED diffusion cylinder-streamlines
)

# ######################################################################### #
