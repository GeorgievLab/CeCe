
# The CeCe simulator

[![Build Status](https://travis-ci.org/GeorgievLab/CeCe.svg?branch=master)](https://travis-ci.org/GeorgievLab/CeCe)
[![Build status](https://ci.appveyor.com/api/projects/status/jdb0jx7g8yiru7w7/branch/master?svg=true)](https://ci.appveyor.com/project/NTSFka/cece/branch/master)

CeCe is a platform for advanced simulation of cells and cell signalling in 2D.
It includes a physical engine, stochastic chemical reactions, cell-cell bonding, diffusion, hydrodynamics, and a growth model.
CeCe originally started as a part of the iGEM 2015 project from team Czech Republic and was aimed for
the simulation of microfluidical chips. However, Cece's wide range of tools enables it to be used for
the simulation of batch cultures, blood streams, cell tissues, and many more biomedically related problems.

> This repository is just a metarepository for other parts of the CeCe project. It's updated only occasionally, see individual repositories for the newest version.
> Repositories: [core](https://github.com/GeorgievLab/CeCe-core), [plugins](https://github.com/GeorgievLab/CeCe-plugins), [CLI](https://github.com/GeorgievLab/CeCe-cli), [examples](https://github.com/GeorgievLab/CeCe-examples)

## Prebuilt packages

GitHub releases contains prebuilt packages (release version) for Windows, Mac OSX and Linux.

## Building from source

Building from source code is done by CMake (see Requirements).

### Linux & OSX

```bash
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
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
* [CMake](https://cmake.org) v3.1 or newer.
* From [plugins](https://github.com/GeorgievLab/CeCe-plugins) repository.
* From [cli](https://github.com/GeorgievLab/CeCe-cli) repository.
