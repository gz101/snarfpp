#!/bin/bash
# docker-entrypoint.sh

# Execute the make command with the passed argument, default to 'all' if none
# specified
make "${@:-all}"
