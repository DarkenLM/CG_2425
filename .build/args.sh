#!/bin/bash

has_param() {
    local n=$1
    local terms=("${@:2:$n}")
    for i in $(seq 0 $n); do shift; done;
    for arg; do
        for i in $(seq 0 $n); do
            if [[ $arg == "${terms[$i]}" ]]; then
                return 0
            fi
        done
    done
    return 1
}

get_args() {
    # local args=()
    local -n args=$1
    shift;
    for arg; do
        if [[ $arg != -* ]]; then
            args+=("$arg")
        fi
    done
    # echo "${args[@]}"
}