
# The CeCe simulator

[![Build Status](https://travis-ci.org/GeorgievLab/CeCe.svg?branch=master)](https://travis-ci.org/GeorgievLab/CeCe)

CeCe is a platform for advanced simulation of cells and cell signalling in 2D. It includes a physical engine, stochastic chemical reactions, cell-cell bonding, diffusion, hydrodynamics, and a growth model. CeCe originally started as a part of the iGEM 2015 project from team Czech Republic and was aimed for the simulation of microfluidical chips. However, Cece's wide range of tools enables it to be used for the simulation of batch cultures, blood streams, cell tissues, and many more biomedically related problems.

## Building from source

Building from source code is done by CMake.

```bash
$ cd $SOURCE
$ mkdir build && cd build
$ cmake ..
$ make
```

### Requirements

* C++11 compiler - GCC, Clang (MSVC is not supported due lack of full C++11 support)
* CMake v2.8 or newer
* Box2D v2.3 or newer
* Boost Filesystem

### Optional requirements

For some functionality and plugins additional libraries are required.

* OpenCV library - image and video capturing; `picture` and `obstacles-image` plugins
* GLFW3 - Simulation visualization
