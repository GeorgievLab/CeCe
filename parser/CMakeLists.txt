# ######################################################################### #

# Project name
set(PROJECT_NAME parser)

# Create project
project(${PROJECT_NAME} CXX)

# Current version is unable to add object library into shared library.
if (UNIX)
    add_compile_options(-fPIC)
endif ()

if (WIN32)
    add_definitions(-DSIMULATOR_DLL_EXPORT=1)
endif ()

# Create library
add_library(${PROJECT_NAME} OBJECT
    Parser.hpp
    Parser.cpp
    SimulationFactory.hpp
    SimulationFactory.cpp
)

# ######################################################################### #
