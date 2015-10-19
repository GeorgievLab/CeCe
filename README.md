
# The CeCe simulator

[![Build Status](https://travis-ci.org/GeorgievLab/CeCe.svg?branch=master)](https://travis-ci.org/GeorgievLab/CeCe)

The CeCe simulator is modular simulator primary designed for modeling signal
transmission networks in microfluidics.

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
