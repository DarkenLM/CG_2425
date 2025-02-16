#!/bin/bash

###################################################################################
# clean.sh
# Cleans the target directory.
#
# FLAGS:
#   --debug, -d: Builds the project in debug mode.
###################################################################################

source .build/args.sh

OUTDIR="build/out"

if has_param 1 '--all' "$@"; then
    echo "NUKING THIS SHIT"
    if [ -d "build" ] ; then
        rm -r build
    fi
else
    if [ -d "$OUTDIR" ] ; then
        rm -r $OUTDIR
    fi
fi