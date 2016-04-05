#!/bin/sh

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

# Add PPAs
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
#sudo add-apt-repository ppa:kalakris/cmake -y
#sudo add-apt-repository ppa:boost/latest -y
#sudo add-apt-repository ppa:llvm-toolchain/precise-3.7 -y

sudo apt-get -qq update

# Update compiler variables
if [ "$TRAVIS_OS_NAME" = "linux" -a "$CXX" = "g++" ]; then 
	export CXX="g++-4.9" CC="gcc-4.9"
fi

if [ "$TRAVIS_OS_NAME" = "linux" -a "$CXX" = "clang++" ]; then 
	export CXX="clang++-3.7" CC="clang-3.7"
fi

# ######################################################################### #

