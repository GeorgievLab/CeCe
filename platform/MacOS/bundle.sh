#!/bin/bash

BINARY=$1

function fix_library()
{
    LIBRARY=$1

    if [[ $LIBRARY == *"/"* ]]
    then
        #install_name_tool -change $BINARY @executable_path/../Frameworks/
        echo "Pass"
    else
        # Local
        install_name_tool -change $LIBRARY @executable_path/../Frameworks/$LIBRARY $BINARY
    fi
}

LIBRARIES=$(otool -L $BINARY | grep -oE "([^ ]*).dylib")

for LIBRARY in $LIBRARIES
do
    fix_library $LIBRARY
done
