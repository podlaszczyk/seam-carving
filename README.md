# Project

This repository contains a working CMake based project you may use for writing your solution.

## Prerequisities

All packages necessary to build the project are defined inside the [Dockerfile](Dockerfile "Dockerfile") (Ubuntu 20.04 based).

### Docker Container

Building the docker image:
```
cd seam-carving
docker build . -t seamcarving

```

Running the docker container:
```
docker run --rm -v (pwd):/workdir -p 2323:2323 -it seamcarving bash
```

port 2323 is custom selected port to enable ssh connection between host and container

The above command maps the current working directory from the host OS to the _/workdir_ directory
inside the docker container.

## Building

Execute the following steps inside the docker container:
```
cd /workdir
cmake -S src -B build -G Ninja
cmake --build build
```

## Running

The following command runs the built _seam-carving_ binary (executed from the docker container):
```
/workdir/build/seam-carving examples/castle.jpg
```

The process requires path to the input image passed as a mandatory command line argument. The generated output image created by your seam-carving algorithm is saved down to the filesystem in the current runtime directory (directory the docker container has been run from) under _output.png_ name. You may open that file from your host OS file to see the result. 

## Implementation

The `seamCarving` function defined inside the [seam_carving.cpp](src/seam_carving.cpp "seam_carving.cpp") file is an entry point to your implementation.

## Examples

Example images are located inside the [examples/](examples/ "Example input images") directory.
