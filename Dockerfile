# Use an official Ubuntu base image
FROM ubuntu:latest

# Install gcc, g++, make, and the Boost libraries
RUN apt-get update && \
    apt-get install -y build-essential libboost-all-dev

# Set the working directory in the container
WORKDIR /usr/src/snarfpp

# Copy the entry point script and make it executable
COPY docker-entrypoint.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

# Set the entry point script to run on container start
ENTRYPOINT ["docker-entrypoint.sh"]
