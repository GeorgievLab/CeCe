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

if (BUILD_TESTS)
    # Create executable
    add_executable("${LIBRARY_NAME}_test"
        test/test.cpp
        test/ParserTest.cpp
        test/ReactionTest.cpp
    )
    target_link_libraries("${LIBRARY_NAME}_test"
        ${PROJECT_NAME}
        ${GTEST_BOTH_LIBRARIES}
    )

    if (UNIX)
        target_link_libraries("${LIBRARY_NAME}_test"
            -pthread
        )
    endif ()

    # Add test
    add_test(${PROJECT_NAME} "${LIBRARY_NAME}_test")
endif ()

# ######################################################################### #
