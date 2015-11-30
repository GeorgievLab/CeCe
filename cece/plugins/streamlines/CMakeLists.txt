# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
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

# Options
option(DEV_PLUGIN_streamlines_SWAP_TRICK "Enable or disable streamlines swap-trick for computation speedup" Off)

if (CONFIG_RENDER_ENABLE)
    option(DEV_PLUGIN_streamlines_RENDER "Enable or disable streamlines rendering" Off)
endif ()

# ######################################################################### #

if (DEV_PLUGIN_streamlines_SWAP_TRICK)
    add_definitions(-DDEV_PLUGIN_streamlines_SWAP_TRICK=1)
endif ()

# Enable debug rendering
if (DEV_PLUGIN_streamlines_RENDER)
    add_definitions(-DDEV_PLUGIN_streamlines_RENDER=1)
endif ()

# ######################################################################### #

# Sources
set(SRCS
    Plugin.cpp
    Module.hpp
    Module.cpp
    Lattice.hpp
    Lattice.cpp
    LatticeCell.hpp
    LatticeCell.cpp
)

# Build plugin
build_plugin(streamlines
    SOURCES ${SRCS}
)

# ######################################################################### #
