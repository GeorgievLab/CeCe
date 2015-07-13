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
    Reaction.hpp
    Reaction.cpp
    ReactionIntracellular.hpp
    ReactionIntracellular.cpp
    ReactionParser.hpp
    ReactionParser.cpp
    ReactionParserIntracellular.hpp
    ReactionParserIntracellular.cpp
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
        test/ParserTest.cpp
    PLUGINS_REQUIRED
        stochastic-reactions
)

# ######################################################################### #
