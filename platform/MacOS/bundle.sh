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
SOURCE_DIR=/usr/local/lib

# Directory of the binary
DIRECTORY=`dirname $1`

#
# Get dependency of given library
#
# $1: Path to binary.
#
function get_dependency()
{
    otool -L $1 | grep -oE "([^ ]*).dylib"
}

#
# Fix libraries
#
# $1: Path to original binary/library.
# $2: Path to copy binary/library.
# $3: Path to library that needs to be fixed.
#
function fix_libraries()
{
    local BUNDLE_PATH="${DIRECTORY}/${FRAMEWORKS}"

    local BINARY=$1
    local BINARY_COPY=$2

    # Foreach binary dependencies
    for LIBRARY in `get_dependency $BINARY`
    do
        if [[ $LIBRARY == *"@executable_path"* ]]
        then
            # Skip paths with @executable_path
            continue
        fi

        # If library contains separator is from system
        if [[ $LIBRARY == *"/"* ]]
        then
            local BASENAME=`basename ${LIBRARY}`
            local FILEPATH="${BUNDLE_PATH}/${BASENAME}"

            # Only libraries from sources
            # /usr/local/lib
            if [[ $LIBRARY == *"${SOURCE_DIR}"* ]]
            then

                echo "Library: '${LIBRARY}'"

                if [[ ! -f "${FILEPATH}" ]]
                then
                    # Copy library into bundle
                    #echo "Copy '${LIBRARY}' to '${BUNDLE_PATH}'"
                    cp "${LIBRARY}" "${BUNDLE_PATH}"

                    # Recursive fix
                    fix_libraries $LIBRARY $FILEPATH
                fi

                echo "Update '${BINARY#$DIRECTORY}' for '${LIBRARY}'"
                chmod a+w ${BINARY}
                install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${BASENAME} ${BINARY_COPY}
                chmod a-w ${BINARY}

                echo "----------"
            fi
        else
            echo "Local library: '${LIBRARY}'"

            # Local
            echo "Local update '${BINARY#$DIRECTORY}' for '${LIBRARY}'"
            install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${LIBRARY} ${BINARY_COPY}

            # Recursive fix
            fix_libraries $LIBRARY $BUNDLE_PATH/$LIBRARY

            echo "----------"
        fi
    done
}

# Source binary
fix_libraries $1 $1

# ######################################################################### #
