# ######################################################################### #

# Project name
set(PROJECT_NAME module-cell)

# Create project
project(${PROJECT_NAME} CXX)

# Find Boost
find_package(Boost REQUIRED)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})

# Sources
set(SRCS
    Library.hpp
    Library.cpp
    CellBase.hpp
    CellBase.cpp
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

    # Shaders
    vs.cell.hpp
    fs.cell.hpp
    vs.yeast.hpp
    fs.yeast.hpp
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
)

# ######################################################################### #
