# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

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

# Build plugin
build_plugin(cell
    SOURCES ${SRCS}
)

# ######################################################################### #
