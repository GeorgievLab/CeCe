# ######################################################################### #

# Project name
set(PROJECT_NAME module-python)

# Create project
project(${PROJECT_NAME} CXX)

# Include Python API
find_package(PythonLibs REQUIRED)

# Include Boost filesystem and Python
find_package(Boost REQUIRED COMPONENTS filesystem system python)

# Include directories
include_directories(
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

# Sources
set(SRCS
    Module.hpp
    Module.cpp
    Library.hpp
    Library.cpp
)

# Create library
if (MODULE_BUILD_STATIC)
    add_library(${PROJECT_NAME} ${SRCS})
else ()
    add_library(${PROJECT_NAME} SHARED ${SRCS})
endif ()

# Setup dependencies
add_dependencies(${PROJECT_NAME}
    core
    simulator
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    simulator
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
)

# ######################################################################### #
