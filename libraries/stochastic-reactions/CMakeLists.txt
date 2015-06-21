# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

# Library name
set(LIBRARY_NAME stochastic-reactions)

# Project name
set(PROJECT_NAME "module-${LIBRARY_NAME}")

# Create project
project(${PROJECT_NAME} CXX)

# Sources
set(SRCS
    Library.hpp
    Library.cpp
    Reaction.hpp
    Reaction.cpp
)

# Build static / shared
list(FIND MODULES_BUILDIN "${LIBRARY_NAME}" IS_STATIC)

if (${IS_STATIC} GREATER -1)
    message("-- Library ${LIBRARY_NAME} is build-in")

    if (UNIX)
        add_compile_options(-fPIC)
    endif ()

    add_definitions(-DLIBRARY_BUILDIN=1)
    add_library(${PROJECT_NAME} ${SRCS})

    # Setup dependencies
    add_dependencies(${PROJECT_NAME}
        core
    )
else ()
    message("-- Library ${LIBRARY_NAME} is shared")
    add_library(${PROJECT_NAME} SHARED ${SRCS})

    # Setup dependencies
    add_dependencies(${PROJECT_NAME}
        core
        simulator
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        simulator
    )

    # Install
    install(TARGETS ${PROJECT_NAME}
        DESTINATION lib
    )
endif ()

# ######################################################################### #

if (BUILD_TESTS)
    # Create executable
    add_executable("${LIBRARY_NAME}_test"
        test/test.cpp
        test/ParserTest.cpp
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
