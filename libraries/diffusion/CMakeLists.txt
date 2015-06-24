# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Sources
set(SRCS
    Library.hpp
    Library.cpp
    Signal.hpp
    Module.hpp
    Module.cpp
    Generator.hpp
    Generator.cpp
    SignalGrid.hpp
)

# Build plugin
build_plugin(diffusion
    SOURCES ${SRCS}
)

# ######################################################################### #
