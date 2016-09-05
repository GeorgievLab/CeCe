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

cmake_minimum_required(VERSION 3.1)

# ######################################################################### #

option(CECE_BUILD_CLI "Build CLI application" On)

# ######################################################################### #

add_subdirectory(core)
add_subdirectory(plugins)

if (CECE_BUILD_CLI)
    add_subdirectory(app/cli)
endif ()

# ######################################################################### #

# Build unit tests
# CECE_TESTS_BUILD is an option in CeCe-core
if (CECE_TESTS_BUILD)
    enable_testing()
endif ()

# ######################################################################### #

