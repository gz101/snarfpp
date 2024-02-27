#!/bin/bash

docker run --name snarfpp_container -v $(pwd):/usr/src/snarfpp snarfpp tests
docker rm snarfpp_container
make clean
