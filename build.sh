#!/bin/bash

###################################################################################
# build.sh
# Configures and executes the projects as defined by their CMakeLists.
#
# FLAGS:
#   --debug, -d: Builds the project in debug mode.
###################################################################################

source .build/args.sh

FLAGS=""

if has_param 2 '--verbose' '-v' "$@"; then FLAGS="$FLAGS --trace-expand"; fi

if has_param 2 '--debug' '-d' "$@"; then
    echo "Running in DEBUG mode." 
    cmake $FLAGS -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
else
    echo "Running in RELEASE mode." 
    cmake $FLAGS -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build
fi
