# ######################################################################### #
# Georgiev Lab (c) 2015-2016                                                #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #
#                                                                           #
# This file is part of CeCe.                                                #
#                                                                           #
# CeCe is free software: you can redistribute it and/or modify              #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation, either version 3 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# CeCe is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with CeCe.  If not, see <http://www.gnu.org/licenses/>.             #
#                                                                           #
# ######################################################################### #

# Sources
set(SRCS
    Plugin.cpp
    CellBase.hpp
    CellBase.cpp
    Cell.hpp
    Cell.cpp
    Yeast.hpp
    Yeast.cpp
    StoreMolecules.hpp
    StoreMolecules.cpp
)

if (CONFIG_RENDER_ENABLE)
    set(SRCS ${SRCS}
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
endif ()

# ######################################################################### #

# Build plugin
build_plugin(cell
    SOURCES ${SRCS}
)

# ######################################################################### #
