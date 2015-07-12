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

# Get dependency of given library
function get_dependency()
{
    otool -L $1 | grep -oE "([^ ]*).dylib"
}

# Fix binary libraries
function fix_libraries()
{
    local BINARY=$1
    local LIBRARY=1

    echo "Binary: '${BINARY}'"

    for LIBRARY in `get_dependency $BINARY`
    do
        fix_library $BINARY $LIBRARY
    done

    echo "----------"
}

# Fix library
function fix_library()
{
    local BINARY=$1
    local LIBRARY=$2

    if [[ $LIBRARY == *"/"* ]]
    then
        local BASENAME=`basename ${LIBRARY}`
        local BUNDLE_PATH="${DIRECTORY}/${FRAMEWORKS}"
        local FILEPATH="${BUNDLE_PATH}/${BASENAME}"

        # Only libraries from sources
        if [[ $LIBRARY == *"${SOURCE_DIR}"* ]]
        then

            echo "Library: '${LIBRARY}'"

            if [[ ! -f "${FILEPATH}" ]]
            then
                # Copy library into bundle
                #echo "Copy '${LIBRARY}' to '${BUNDLE_PATH}'"
                cp "${LIBRARY}" "${BUNDLE_PATH}"

                # Recursive fix
                fix_libraries $FILEPATH
            fi

            echo "Update '${BINARY#$DIRECTORY}' for '${LIBRARY}'"
            chmod a+w ${BINARY}
            install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${BASENAME} ${BINARY}
            chmod a-w ${BINARY}

            echo "----------"
        fi
    else

        echo "Local library: '${LIBRARY}'"

        # Local
        echo "Local update '${BINARY#$DIRECTORY}' for '${LIBRARY}'"
        install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${LIBRARY} ${BINARY}

        echo "----------"
    fi
}

# Source binary
fix_libraries $1

# ######################################################################### #
