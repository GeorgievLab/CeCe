# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

if (DEV_TEST_EXAMPLES AND ENABLE_CLI)
    # Get all examples
    file(GLOB EXAMPLES RELATIVE "${CMAKE_SOURCE_DIR}/examples"
        "${CMAKE_SOURCE_DIR}/examples/*.(xml|reactions)"
    )

    # Foreach examples
    foreach (EXAMPLE ${EXAMPLES})
        add_test(
            NAME "test_${EXAMPLE}"
            COMMAND $<TARGET_FILE:cli> "${CMAKE_SOURCE_DIR}/examples/${EXAMPLE}"
        )

    endforeach ()
endif ()

# ######################################################################### #

