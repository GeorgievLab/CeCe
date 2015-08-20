# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

option(CONFIG_PLUGIN_cell_BFP_COLOR "Enable coloring cell by BFP protein" On)

# ######################################################################### #

if (CONFIG_PLUGIN_cell_BFP_COLOR)
    add_definitions(-DCONFIG_PLUGIN_cell_BFP_COLOR=1)
endif ()

# ######################################################################### #

# Sources
set(SRCS
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
    StoreMolecules.hpp
    StoreMolecules.cpp

    # Shaders
    vs.cell.hpp
    fs.cell.hpp
    vs.yeast.hpp
    fs.yeast.hpp
)

# Build plugin
build_plugin(cell
    SOURCES ${SRCS}
)

# ######################################################################### #
