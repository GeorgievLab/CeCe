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

# Install dependencies
brew outdated glfw3 || brew upgrade glfw3
brew outdated box2d || brew upgrade box2d
brew outdated libpng || brew upgrade libpng
brew outdated gtest || brew upgrade gtest

# Build GTest
if [ ! -d $VENDOR_DIR/gtest ]; then 
	wget https://github.com/google/googletest/archive/release-1.7.0.zip -O gtest.zip && 
	unzip gtest.zip -d . && 
	cd googletest-release-1.7.0 &&
	cmake -DCMAKE_INSTALL_PREFIX=$VENDOR_DIR/gtest -DCMAKE_BUILD_TYPE=release . &&
	cmake --build . && mkdir -p $VENDOR_DIR/gtest &&
	cp -r include $VENDOR_DIR/gtest && mkdir -p $VENDOR_DIR/gtest/lib && cp libgtest*.a $VENDOR_DIR/gtest/lib && cd -; 
fi

# ######################################################################### #
