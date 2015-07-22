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

CWD=`dirname $0`

# Include bundle script
. $CWD/bundle.sh

PLUGINS=../PlugIns

# Directory of the binary
DIRECTORY=`dirname $1`

#
# Get dependency of given library on other plugins
#
# $1: Path to binary.
#
function get_plugin_dependency()
{
    # Shared library, remove the first line that contains ID
    otool -L $1 | tail -n+2 | grep -oE "([^ ]*)libplugin(.*).dylib" | tail -n+2 | grep -v "@executable_path"
}

#
# Fix libraries
#
# $1: Path to binary/library.
#
function fix_plugin()
{
    local PLUGIN_PATH="${DIRECTORY}/${PLUGINS}"

    local BINARY=$1
    local LIBRARY

    debug "PLUGIN_START: $BINARY\n"

    # Get library ID, is empty for
    local ID=`get_id $BINARY`

    debug "ID = ${ID}"

    # Fix binary ID (only shared libraries)
    if [[ -z "${ID}" ]]
    then
        echo "Missing ID"
        exit 1
    fi

    local BASENAME=`basename ${BINARY}`

    # Update ID
    debug "ID:\ninstall_name_tool -id\n@executable_path/${PLUGINS}/${BASENAME}\n${BINARY}\n"

    chmod u+w ${BINARY}
    install_name_tool -id @executable_path/${PLUGINS}/${BASENAME} ${BINARY}
    chmod u-w ${BINARY}

    # Foreach plugin dependencies
    for PLUGIN in `get_plugin_dependency "${BINARY}"`
    do
        echo "##### " $PLUGIN
        # Plugins are in same directory without path
        debug "PLUGIN:\ninstall_name_tool -change\n${PLUGIN}\n@executable_path/${PLUGINS}/${PLUGIN}\n${BINARY}\n"

        chmod u+w ${BINARY}
        install_name_tool -change ${PLUGIN} @executable_path/${PLUGINS}/${PLUGIN} ${BINARY}
        chmod u-w ${BINARY}
    done

    # Fix as binary
    fix_binary $BINARY

    debug "PLUGIN_END: $BINARY\n\n"
}

# Source binary
fix_plugin $1

# ######################################################################### #
