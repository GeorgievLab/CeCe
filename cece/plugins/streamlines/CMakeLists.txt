# ######################################################################### #
# Georgiev Lab (c) 2016                                                     #
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

# Enable swap trick
option(CECE_PLUGIN_streamlines_SWAP_TRICK "Enable OpenLB swap trick improvement" On)

# ######################################################################### #

# Create config file
configure_file(config.hpp.in config.hpp)

# ######################################################################### #

# Sources
set(SRCS
    Plugin.cpp
    Descriptor.hpp
    Descriptor.cpp
    Module.hpp
    Module.cpp
    Lattice.hpp
    Lattice.cpp
    Node.hpp
    Dynamics.hpp
    Dynamics.cpp
    NoDynamics.hpp
    NoDynamics.cpp
    BgkDynamics.hpp
    BgkDynamics.cpp
    BounceBackDynamics.hpp
    BounceBackDynamics.cpp
    ZouHeDynamics.hpp
    ZouHeDynamics.cpp
)

# Build plugin
build_plugin(streamlines
    SOURCES ${SRCS}
)

# ######################################################################### #
