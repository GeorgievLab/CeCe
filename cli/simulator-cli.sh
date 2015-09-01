#!/bin/bash

# App name
NAME=simulator-cli

# Current directory
BASEDIR=`dirname $0`

# Path to binary
BINARY=${BASEDIR}/${NAME}.app/Contents/MacOS/${NAME}

# Set working directory
export WORKING_DIR=`pwd`

# Run binary
${BINARY} $@
