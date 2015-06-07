#!/bin/sh

BUNDLE="`echo "$0" | sed -e 's|/Contents/MacOS/.*||'`"
RESOURCES="$BUNDLE/Contents/Resources"

export "DYLD_LIBRARY_PATH=$RESOURCES/lib"
export "LD_LIBRARY_PATH=$RESOURCES/lib"
export "PATH=$RESOURCES/bin:$PATH"

exec "$RESOURCES/bin/cell-sim-gui"
