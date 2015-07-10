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
)

# Build plugin
build_plugin(stochastic-reactions-diffusive
    SOURCES ${SRCS}
    PLUGINS_REQUIRED cell stochastic-reactions diffusion
)

# ######################################################################### #

# Add tests
build_test(stochastic-reactions-diffusive
    SOURCES
        test/test.cpp
    PLUGINS_REQUIRED
        stochastic-reactions
        diffusion
)

# ######################################################################### #
