# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.hpp
    Library.cpp
)

# Build plugin
build_plugin(cylinder-streamlines
    SOURCES ${SRCS}
)

# ######################################################################### #
