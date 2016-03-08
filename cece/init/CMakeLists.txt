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

# Initialization project
project(init CXX)

# ######################################################################### #

# Sources
set(SRCS
    Initializer.hpp
    Container.hpp
    Container.cpp
    Factory.hpp
    FactoryManager.hpp
    FactoryManager.cpp
)

# ######################################################################### #

# Create library
add_library(${PROJECT_NAME} OBJECT ${SRCS})

# ######################################################################### #

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE On)

# ######################################################################### #

# Install header files
install(DIRECTORY .
    DESTINATION ${INSTALL_DIR_INCLUDE}/${CMAKE_PROJECT_NAME}/${PROJECT_NAME}
    COMPONENT Headers
    FILES_MATCHING PATTERN "*.hpp"
)

# ######################################################################### #
