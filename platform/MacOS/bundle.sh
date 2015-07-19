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

#DEBUG_FILE="update.log"

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
    local LIBRARY

    #echo -e "START: $BINARY\n" >> $DEBUG_FILE

    # Foreach binary dependencies
    for LIBRARY in `get_dependency $BINARY_COPY`
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
                if [[ ! -f "${FILEPATH}" ]]
                then
                    # Copy library into bundle
                    #echo "Copy '${LIBRARY}' to '${BUNDLE_PATH}'"
                    cp "${LIBRARY}" "${BUNDLE_PATH}"

                    # Recursive fix
                    fix_libraries $LIBRARY $FILEPATH
                fi

                if [[ $LIBRARY == $BINARY ]]
                then
                    chmod a+w ${BINARY_COPY}
                    install_name_tool -id @executable_path/${FRAMEWORKS}/${BASENAME} ${BINARY_COPY}
                    chmod a-w ${BINARY_COPY}

                    #echo -e "install_name_tool -id\n@executable_path/${FRAMEWORKS}/${BASENAME}\n${BINARY_COPY}\n" >> $DEBUG_FILE
                else
                    chmod a+w ${BINARY_COPY}
                    install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${BASENAME} ${BINARY_COPY}
                    chmod a-w ${BINARY_COPY}

                    #echo -e "install_name_tool -change\n${LIBRARY}\n@executable_path/${FRAMEWORKS}/${BASENAME}\n${BINARY_COPY}\n" >> $DEBUG_FILE
                fi
            fi
        else
            if [[ $LIBRARY == $BINARY ]]
            then
                chmod a+w ${BINARY_COPY}
                install_name_tool -id @executable_path/${FRAMEWORKS}/${BASENAME} ${BINARY_COPY}
                chmod a-w ${BINARY_COPY}

                #echo -e "LOCAL:\ninstall_name_tool -id\n@executable_path/${FRAMEWORKS}/${BASENAME}\n${BINARY_COPY}\n" >> $DEBUG_FILE
            else
                # Local
                install_name_tool -change ${LIBRARY} @executable_path/${FRAMEWORKS}/${LIBRARY} ${BINARY_COPY}

                #echo -e "LOCAL:\ninstall_name_tool -change\n${LIBRARY}\n@executable_path/${FRAMEWORKS}/${LIBRARY}\n${BINARY_COPY}\n" >> $DEBUG_FILE

                # Recursive fix
                fix_libraries $LIBRARY $BUNDLE_PATH/$LIBRARY
            fi
        fi
    done

    #echo -e "END: $BINARY\n\n" >> $DEBUG_FILE
}

#echo "" > $DEBUG_FILE

# Source binary
fix_libraries $1 $1

# ######################################################################### #
