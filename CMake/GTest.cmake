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

cmake_minimum_required(VERSION 2.8)

include(ExternalProject)
ExternalProject_Add(GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    CMAKE_ARGS -DBUILD_GMOCK:BOOL=Off -DBUILD_GTEST:BOOL=On
    PREFIX "${CMAKE_BINARY_DIR}"
    # Disable installation
    INSTALL_COMMAND ""
)

# Specify directories
ExternalProject_Get_Property(GTest SOURCE_DIR BINARY_DIR)
set(GTEST_INCLUDE_DIRS ${SOURCE_DIR}/googletest/include)
set(GTEST_LIB_DIRS ${BINARY_DIR}/googletest)
set(GTEST_BOTH_LIBRARIES gtest gtest_main)

# ######################################################################### #
