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

if (CONFIG_RENDER_ENABLE)
    option(CONFIG_PLUGIN_diffusion_SMOOTH "Render smooth diffusion" On)
    option(DEV_PLUGIN_diffusion_OBSTACLES_RENDER "Render obstacles" Off)
endif ()

# ######################################################################### #

if (CONFIG_PLUGIN_diffusion_SMOOTH)
    add_definitions(-DCONFIG_PLUGIN_diffusion_SMOOTH=1)
endif ()

if (DEV_PLUGIN_diffusion_OBSTACLES_RENDER)
    add_definitions(-DDEV_PLUGIN_diffusion_OBSTACLES_RENDER=1)
endif ()

# ######################################################################### #

# Sources
set(SRCS
    Library.cpp
    Module.hpp
    Module.cpp
    Generator.hpp
    Generator.cpp
    StoreState.hpp
    StoreState.cpp
)

# ######################################################################### #

# Build plugin
build_plugin(diffusion
    SOURCES ${SRCS}
)

# ######################################################################### #
