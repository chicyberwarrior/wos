#!/bin/sh

echo "Building...."
./scripts/build-stage1.sh

if [ $? -eq 0 ]
then
    echo "Running..."
    ./scripts/debug.sh
else
    echo "Go fix your code..."
fi

