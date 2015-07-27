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
    Diffusion.hpp
    Diffusion.cpp
    IntercellularReactions.hpp
    IntercellularReactions.cpp
)

# Build plugin
build_plugin(stochastic-reactions-intercellular
    SOURCES ${SRCS}
    PLUGINS_REQUIRED cell stochastic-reactions-intracellular diffusion
)

# ######################################################################### #

# Add tests
build_test(stochastic-reactions-intercellular
    SOURCES
        test/test.cpp
    PLUGINS_REQUIRED
        stochastic-reactions-intracellular
        diffusion
)

# ######################################################################### #
