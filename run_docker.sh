#!/bin/bash

# CLI argument needs to be one of [tests, all, examples]
if ! ([ "$1" = "tests" ] || [ "$1" = "all" ] || [ "$1" = "examples" ]); then
    echo "ERROR: Invalid argument, can only be one of {tests, all, examples}."
    exit 1
fi

# execute unit tests
docker run --name snarfpp_container -v $(pwd):/usr/src/snarfpp -u $(id -u):\
$(id -g) snarfpp $1

# clean up container and files
make clean
docker rm snarfpp_container

echo "Finished execution successfully!"
