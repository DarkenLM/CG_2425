#!/bin/bash

###################################################################################
# run.sh
# Runs the built executables.
#
# FLAGS:
#   --debug, -d: Rebuilds the project in debug mode (does nothing with --no-make).
#   --no-make: Runs the executable directly.
###################################################################################

source .build/args.sh
# args="$(get_args "$@")"
# args=$(get_args "$@")
get_args args "$@"

run() {
    # echo "ARGSI: ${!args[@]}"
    # echo "ARGS: ${args[@]} ${args[1]}"
    # echo "COMMAND: './build/out/bin/${args[0]} ${args[@]:1}'"
    eval "./build/out/bin/${args[0]} ${args[@]:1}"
}

run_with_cmake() {
    if has_param 2 '--debug' '-d' "$@"; then
        echo "Running in DEBUG mode." 
        cmake -B build -DCMAKE_BUILD_TYPE=Debug
        # cmake --build build --target run_${args[0]}
        cmake --build build --target ${args[0]}
        run;
    else
        echo "Running in RELEASE mode." 
        cmake -B build -DCMAKE_BUILD_TYPE=Release
        # cmake --build build --target run_${args[0]} "${args[@]:1}"
        run;
    fi
}

if has_param 1 '--no-make' "$@"; then
    run
else
    run_with_cmake $@;
fi


# cmake --build build --target run_$1
