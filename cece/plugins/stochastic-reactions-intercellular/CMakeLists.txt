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
    Library.cpp
    Diffusion.hpp
    Diffusion.cpp
    IntercellularReactions.hpp
    IntercellularReactions.cpp
)

# Build plugin
build_plugin(stochastic-reactions-intercellular
    SOURCES ${SRCS}
    PLUGINS_REQUIRED cell stochastic-reactions-intracellular diffusion
)

# ######################################################################### #

# Add tests
#build_test(stochastic-reactions-intercellular
#    SOURCES
#        test/test.cpp
#    PLUGINS_REQUIRED
#        stochastic-reactions-intracellular
#        diffusion
#)

# ######################################################################### #
