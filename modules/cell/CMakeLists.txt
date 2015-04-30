# ######################################################################### #

# Project name
set(PROJECT_NAME module-cell)

# Create project
project(${PROJECT_NAME} CXX)

# Sources
set(SRCS
    Library.hpp
    Library.cpp
    ModuleCells.hpp
    ModuleCells.cpp
    Cell.hpp
    Cell.cpp
    Yeast.hpp
    Yeast.cpp
    Generator.hpp
    Generator.cpp
    DrawableCell.hpp
    DrawableCell.cpp
    DrawableYeast.hpp
    DrawableYeast.cpp
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
)

# ######################################################################### #
