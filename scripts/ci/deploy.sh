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

SUFFIX=""

# Add suffix for non-tag releases
if [ -z "$TRAVIS_TAG" ]; then
    SUFFIX="$TRAVIS_BRANCH-$TRAVIS_COMMIT"
fi

# Create a package and deploy
function pack_and_deploy
{
	PLATFORM=$1
	GENERATOR=$2
	EXT=$3

	# Create a package
	cpack -G $GENERATOR

	PACKAGE=CeCe-$VERSION-$PLATFORM.$EXT

	if [ -n "$SUFFIX" ]; then
		PACKAGE_NEW=CeCe-$VERSION-$PLATFORM-$SUFFIX.$EXT
		mv $PACKAGE $PACKAGE_NEW
		PACKAGE=$PACKAGE_NEW
	fi

	echo "Deploy package: $PACKAGE"
	curl --ftp-create-dirs --ftp-ssl -u $FTP_USER:$FTP_PASSWORD ftp://$FTP_SERVER/bin/ -T $PACKAGE || { echo "Deploy failed"; exit 1; }
}

# ######################################################################### #

