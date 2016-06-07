#!/bin/bash

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

SOURCE_DIR=/usr/local/

# Directory of the binary
DIRECTORY=`dirname $1`
LIBRARY_DIRECTORY="${DIRECTORY}/../lib"

# ######################################################################### #

#
# Returns library ID. In case of executable, nothing is returned.
#
# @param: Path to binary.
#
function get_id()
{
    otool -D $1 | tail -n+2
}

# ######################################################################### #

#
# Get dependency of given library
#
# @param: Binary ID. Can be empty.
# @param: Path to binary.
#
function get_dependency()
{
    if [[ -n "$1" ]]
    then
        # Shared library, remove the first line that contains ID
        otool -L $2 | tail -n+2 | grep -oE "([^ ]*).dylib" | tail -n+2
    else
        # Executable
        otool -L $2 | tail -n+2 | grep -oE "([^ ]*).dylib"
    fi
}

# ######################################################################### #

#
# Get path to real file from symbolic links.
#
# @param: Original file
#
function follow_symlink()
{
    local LIBRARY=$1

    # Get real path
    local REALPATH="${LIBRARY}"

    while [ -h "${REALPATH}" ]
    do
        local DIR=`dirname ${REALPATH}`
        local LINK=`readlink ${REALPATH}`
        REALPATH="${DIR}/${LINK}"
    done

    echo $REALPATH
}

# ######################################################################### #

#
# Fix binary
#
# @param: Path to binary/library.
#
function fix_binary()
{
    local BINARY=$1
    local LIBRARY

    # Get library ID, is empty for executable
    local ID=`get_id $BINARY`

    # Fix binary ID (only shared libraries)
    if [[ -n "${ID}" ]]
    then
        local BASENAME=`basename ${BINARY}`

        chmod a+w ${BINARY}
        install_name_tool -id @rpath/${BASENAME} ${BINARY}
        chmod a-w ${BINARY}
    fi

    # Foreach binary dependencies
    for LIBRARY in `get_dependency "${ID}" "${BINARY}"`
    do
        # If library contains separator is from system
        if [[ $LIBRARY == *"/"* ]]
        then
            # Get real path
            local REALPATH=`follow_symlink "${LIBRARY}"`

            local BASENAME=`basename ${REALPATH}`
            local FILEPATH="${LIBRARY_DIRECTORY}/${BASENAME}"

            # Only libraries from sources
            # /usr/local
            if [[ $LIBRARY == *"${SOURCE_DIR}"* ]]
            then
                if [[ ! -f "${FILEPATH}" ]]
                then
                    # Copy library (realpath) into bundle
                    cp "${REALPATH}" "${LIBRARY_DIRECTORY}"

                    # Recursive fix
                    fix_binary $FILEPATH
                fi

                chmod a+w ${BINARY}
                install_name_tool -change ${LIBRARY} @rpath/${BASENAME} ${BINARY}
                chmod a-w ${BINARY}
            fi
        else
            # Local
            chmod u+w ${BINARY}
            install_name_tool -change ${LIBRARY} @rpath/${LIBRARY} ${BINARY}
            chmod u-w ${BINARY}

            # Recursive fix
            fix_binary ${LIBRARY_DIRECTORY}/${LIBRARY}
        fi
    done
}

# ######################################################################### #

# Source binary
fix_binary $1

# ######################################################################### #
