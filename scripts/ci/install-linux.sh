#!/bin/bash

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

sudo apt-get install -qq \
	g++-4.9 \
	clang-3.6 \
	libboost-filesystem1.55-dev \
	xorg-dev \
	libgl1-mesa-dev \
	libpython2.7-dev \
	libbox2d-dev

# Install GLFW3 (not in repo)
git clone https://github.com/glfw/glfw.git -b 3.1.2 glfw3

pushd glfw3

# Configure
cmake \
	-DCMAKE_BUILD_TYPE=release \
	-DGLFW_BUILD_DOCS=Off \
	-DGLFW_BUILD_EXAMPLES=Off \
	-DGLFW_BUILD_TESTS=Off . || exit 1

# Build
cmake --build . || exit 1

# Install
sudo make install || exit 1

popd

# ######################################################################### #

