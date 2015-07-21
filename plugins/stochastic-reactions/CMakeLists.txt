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
    Reactions.hpp
    IntracellularReactions.hpp
    IntracellularReactions.cpp
    ReactionsParser.hpp
)

# Build plugin
build_plugin(stochastic-reactions
    SOURCES ${SRCS}
    PLUGINS_REQUIRED cell
)

# ######################################################################### #

# Add tests
build_test(stochastic-reactions
    SOURCES
        test/test.cpp
        test/ReactionsParserTest.cpp
    PLUGINS_REQUIRED
        stochastic-reactions
)

# ######################################################################### #
