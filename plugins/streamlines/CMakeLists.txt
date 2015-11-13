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
if (CONFIG_RENDER_ENABLE)
    option(DEV_PLUGIN_streamlines_RENDER "Enable or disable streamlines rendering" Off)
    option(DEV_PLUGIN_streamlines_VELOCITY_RENDER "Enable or disable streamlines velocity rendering" Off)
    option(DEV_PLUGIN_streamlines_FORCE_RENDER "Enable or disable streamlines force rendering" Off)
endif ()

# ######################################################################### #

# Enable debug rendering
if (DEV_PLUGIN_streamlines_RENDER)
    add_definitions(-DDEV_PLUGIN_streamlines_RENDER=1)
endif ()

if (DEV_PLUGIN_streamlines_VELOCITY_RENDER)
    add_definitions(-DDEV_PLUGIN_streamlines_VELOCITY_RENDER=1)
endif ()

if (DEV_PLUGIN_streamlines_FORCE_RENDER)
    add_definitions(-DDEV_PLUGIN_streamlines_FORCE_RENDER=1)
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
    StoreState.hpp
    StoreState.cpp
)

# Build plugin
build_plugin(streamlines
    SOURCES ${SRCS}
)

# ######################################################################### #
