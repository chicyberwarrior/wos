#!/bin/sh

echo "Building...."
./scripts/build-stage01.sh

if [ $? -eq 0 ]
then
    echo "Running..."
    ./scripts/debug.sh
else
    echo "Go fix your code..."
fi

