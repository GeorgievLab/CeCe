#!/bin/bash

# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #
# Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                   #
# ######################################################################### #

FRAMEWORKS=../Frameworks
SOURCE_DIR=/usr/local

# Directory of the binary
DIRECTORY=`dirname $1`

DEBUG_FILE="update.log"
echo "" > $DEBUG_FILE

#
# Debug print
#
function debug()
{
    #echo -e $1 >> $DEBUG_FILE
    :
}

#
# Returns library ID. In case of executable, nothing is returned.
#
# $1: Path to binary.
#
function get_id()
{
    otool -D $1 | tail -n+2
}

#
# Get dependency of given library
#
# $1: Binary ID. Can be empty.
# $2: Path to binary.
#
function get_dependency()
{
    if [[ -n "$1" ]]
    then
        # Shared library, remove the first line that contains ID
        otool -L $2 | tail -n+2 | grep -oE "([^ ]*).dylib" | tail -n+2 | grep -v "@executable_path"
    else
        # Executable
        otool -L $2 | tail -n+2 | grep -oE "([^ ]*).dylib" | grep -v "@executable_path"
    fi
}

#
# Get path to real file from symbolic links.
#
# $1: Original file
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

#
# Fix libraries
#
# $1: Path to binary/library.
#
function fix_libraries()
{
    local BUNDLE_PATH="${DIRECTORY}/${FRAMEWORKS}"

    local BINARY=$1
    local LIBRARY

    debug "START: $BINARY\n"

    # Get library ID, is empty for
    local ID=`get_id $BINARY`

    debug "ID = ${ID}"

    # Fix binary ID (only shared libraries)
    if [[ -n "${ID}" ]]
    then
        local BASENAME=`basename ${BINARY}`

        debug "ID:\ninstall_name_tool -id\n@executable_path/${FRAMEWORKS}/${BASENAME}\n${BINARY}\n"

        chmod a+w ${BINARY}
        install_name_tool -id @executable_path/${FRAMEWORKS}/${BASENAME} ${BINARY}
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
            local FILEPATH="${BUNDLE_PATH}/${BASENAME}"

            # Only libraries from sources
            # /usr/local
            if [[ $LIBRARY == *"${SOURCE_DIR}"* ]]
            then
                if [[ ! -f "${FILEPATH}" ]]
                then
                    # Copy library (realpath) into bundle
                    cp "${REALPATH}" "${BUNDLE_PATH}"

                    # Recursive fix
                    fix_libraries $FILEPATH
                fi

                debug "SYSTEM:\ninstall_name_tool -change\n${LIBRARY}\n@executable_path/${FRAMEWORKS}/${BASENAME}\n${BINARY}\n"

                chmod a+w ${BINARY}
                install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${BASENAME} ${BINARY}
                chmod a-w ${BINARY}
            fi
        else

            debug "LOCAL:\ninstall_name_tool -change\n${LIBRARY}\n@executable_path/${FRAMEWORKS}/${LIBRARY}\n${BINARY}\n"

            # Local
            install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${LIBRARY} ${BINARY}

            # Recursive fix
            fix_libraries $BUNDLE_PATH/$LIBRARY
        fi
    done

    debug "END: $BINARY\n\n"
}

# Source binary
fix_libraries $1 $1

# ######################################################################### #
