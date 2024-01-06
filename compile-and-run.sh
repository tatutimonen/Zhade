#!/bin/bash

readonly CUR_DIR=$(basename "$(pwd)")
if [[ $CUR_DIR != "build" ]]; then
    echo "Not a build directory, aborting."
    exit 1
fi

cmake --build . -j "$(nproc)"
./Zhade/Release/Zhade
