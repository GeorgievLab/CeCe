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

# Sources
set(SRCS
    Simulation.hpp
    Simulation.cpp
    Simulator.hpp
    Simulator.cpp
    IterationType.hpp
    IterationRange.hpp
    TimeMeasurement.hpp
    TimeMeasurement.cpp
    Visualization.hpp
    Visualization.cpp
    VisualizationLayer.hpp
    DefaultSimulation.hpp
    DefaultSimulation.cpp
)

if (CONFIG_PHYSICS_BUILTIN)
    # Sources
    set(SRCS ${SRCS}
        ConverterBox2D.hpp
        ConverterBox2D.cpp
    )
endif ()

# ######################################################################### #

dir_pretend(SOURCES simulator/ ${SRCS})

set(SOURCES_SIMULATOR ${SOURCES} PARENT_SCOPE)

# ######################################################################### #
