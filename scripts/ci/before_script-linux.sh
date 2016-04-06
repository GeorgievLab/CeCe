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

# Shared configuration arguments
ARGS="\
	-DCMAKE_BUILD_TYPE=release \
	-DDEV_TESTS_BUILD=On \
	-DDEV_PHYSICS_BUILTIN_DEBUG=On \
	-DDEV_PLUGIN_streamlines_RENDER=On"

# Use gcc 4.9
if [ "$CXX" = "g++" ]; then 
	ARGS="$ARGS \
		-DCMAKE_CXX_COMPILER=g++-4.9 \
		-DCMAKE_CC_COMPILER=gcc-4.9"

	command -v gcc-4.9 > /dev/null 2>&1 || exit 1
	command -v g++-4.9 > /dev/null 2>&1 || exit 1
fi

# Use clang 3.6
if [ "$CXX" = "clang++" ]; then 
	ARGS="$ARGS \
		-DCMAKE_CXX_COMPILER=clang++-3.6 \
		-DCMAKE_CC_COMPILER=clang-3.6"

	command -v clang-3.6 > /dev/null 2>&1 || exit 1
	command -v clang++-3.6 > /dev/null 2>&1 || exit 1
fi

mkdir build || exit 1
pushd build

# Configure project
cmake $ARGS .. || exit 1

popd

# ######################################################################### #

