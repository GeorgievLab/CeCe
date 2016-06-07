
# The CeCe simulator

[![Build Status](https://travis-ci.org/GeorgievLab/CeCe.svg?branch=master)](https://travis-ci.org/GeorgievLab/CeCe)

CeCe is a platform for advanced simulation of cells and cell signalling in 2D.
It includes a physical engine, stochastic chemical reactions, cell-cell bonding, diffusion, hydrodynamics, and a growth model.
CeCe originally started as a part of the iGEM 2015 project from team Czech Republic and was aimed for
the simulation of microfluidical chips. However, Cece's wide range of tools enables it to be used for
the simulation of batch cultures, blood streams, cell tissues, and many more biomedically related problems.

## Building from source

Building from source code is done by CMake (see Requirements).

### Linux & OSX

```bash
$ cd $SOURCE
$ mkdir build && cd build
$ cmake ..
$ make
```

### Windows
```batch
md build
cd build
cmake -G "Visual Studio 14 2015" ..
cmake --build .
```

### Requirements

* C++11 compiler - GCC, Clang, MSVC.
* [CMake](https://cmake.org) v2.8 or newer.
* [Box2D](http://box2d.org) v2.3 or newer.
* [Boost C++ Libraries](http://www.boost.org) - Filesystem, System, Any, Tribool and Variant.

### Optional requirements

For some functionality and plugins additional libraries are required.

* [libpng](http://libpng.org/pub/png/libpng.html) - image capturing, *picture* plugin and CLI application.
* [zlib](http://zlib.net) - required by libpng.
* [GLFW3](http://www.glfw.org) - Simulation visualization.

## Third party libraries
Libraries that are part of CeCe source code.

* [pugixml](http://pugixml.org) (*xml* plugin).
* [nanosvg](https://github.com/memononen/nanosvg) (*obstacles-svg* plugin).
