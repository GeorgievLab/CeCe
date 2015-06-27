# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Sources
set(SRCS
    Library.hpp
    Library.cpp
    Reaction.hpp
    Reaction.cpp
    ReactionParser.hpp
    ReactionParser.cpp
)

# Build plugin
build_plugin(stochastic-reactions
    SOURCES ${SRCS}
    PLUGINS_REQUIRED cell
)

# ######################################################################### #

# Add tests
build_test(stochastic-reactions
    test/test.cpp
    test/ParserTest.cpp
)

# ######################################################################### #
