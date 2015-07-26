#!/bin/bash

# App name
NAME=simulator-gui

# Current directory
BASEDIR=`dirname $0`

# Path to binary
BINARY=${BASEDIR}/${NAME}.app/Contents/MacOS/${NAME}

# Run binary
${BINARY} $@
