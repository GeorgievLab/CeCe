# ######################################################################### #

# Project name
set(PROJECT_NAME module-diffusion)

# Create project
project(${PROJECT_NAME} CXX)

# Sources
set(SRCS
    Library.hpp
    Library.cpp
    Signal.hpp
    Module.hpp
    Module.cpp
    Generator.hpp
    Generator.cpp
    GeneratorCell.hpp
    GeneratorCell.cpp
    SignalGrid.hpp
    SignalGridDrawable.hpp
    SignalGridDrawable.cpp
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
    module-cell
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    module-cell
    simulator
)

# ######################################################################### #
