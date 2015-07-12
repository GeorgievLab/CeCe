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
DIRECTORY=`dirname $BINARY`

# Get dependency of given library
function get_dependency()
{
    otool -L $1 | grep -oE "([^ ]*).dylib"
}

# Fix binary libraries
function fix_libraries()
{
    BINARY=$1

    for LIBRARY in `get_dependency $BINARY`
    do
        fix_library $BINARY $LIBRARY
    done
}

# Fix library
function fix_library()
{
    BINARY=$1
    LIBRARY=$2

    if [[ $LIBRARY == *"/"* ]]
    then
        BASENAME=`basename ${LIBRARY}`
        BUNDLE_PATH="${DIRECTORY}/${FRAMEWORKS}"
        PATH="${BUNDLE_PATH}/${BASENAME}"

        # Only libraries from sources
        if [[ $LIBRARY == "${SOURCE_DIR}"* ]]
        then
            if [[ ! -f "${PATH}" ]]
            then
                # Copy library into bundle
                cp "${LIBRARY}" "${BUNDLE_PATH}"

                # Recursive fix
                fix_libraries $PATH
            fi

            echo "Update '${BINARY}' for '${LIBRARY}'"
            install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${BASENAME} ${BINARY}
        fi
    else
        # Local
        echo "Update '${BINARY}' for '${LIBRARY}'"
        install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${LIBRARY} ${BINARY}
    fi
}

# Source binary
fix_libraries $1

# ######################################################################### #
