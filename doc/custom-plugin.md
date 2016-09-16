---
layout: article
permalink: /documentation/custom-plugin/
title: "Custom plugin"
share: false
categories:
    - documentation
---

CeCe using plugins to provide functionality for simulation and it's possible to add third-party plugins or even create own plugins. Currently only C++ plugins are supported.

## Create a plugin

At first it's recommended to have cloned repository with runnable application (CLI). In directory where the other plugins are create a directory with your plugin name (in this example we create a plugin named dummy).

```bash
$ mkdir dummy && cd dummy
```

In that directory at least 2 files are required: `CMakeLists.txt` and `Plugins.cpp`.

```bash
# CMakeLists.txt
build_plugin(dummy SOURCES Plugin.cpp)
```

```c++
// Plugin.cpp
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"

class DummyApi : public cece::plugin::Api { };

CECE_DEFINE_PLUGIN(dummy, DummyApi)
```

Next step includes the plugin in the build tree. There's a CMake variable `CECE_PLUGINS_CUSTOM` which contains a list of additional plugins (default is empty) that are built with standard plugins. You can set it from terminal or from GUI.

```bash
$ cmake -DCECE_PLUGINS_CUSTOM=dummy ..
```

You can obtain more information from [standard plugins repository](https://github.com/GeorgievLab/CeCe-plugins) and [Plugin API](/documentation/plugin-api/).

## CMake functions

Each plugin uses `build_plugin` function in `CMakeLists.txt`. This function offers parameters which can configure built plugin.

```
build_plugin(<name> [EXPORT_HEADER]
             SOURCES source1 [source2 ...]
             [LIBRARIES library1 [library2 ...]]
             [DEPENDENCIES dependency1 [dependency2 ...]]
             [PLUGINS_REQUIRED plugin1 [plugin2 ...]])
```

Builds a plugin with name `<name>` from source files. `EXPORT_HEADER` flag generate export header file using `GenerateExportHeader`. Result library depends on given libraries and is built after given dependencies. Plugin can dependent on other plugins (`PLUGINS_REQUIRED`).

There's support for automated tests (googletest) for plugins.

```
build_test(<name>
           SOURCES source1 [source2 ...]
           [LIBRARIES library1 [library2 ...]]
           [DEPENDENCIES dependency1 [dependency2 ...]]
           [PLUGINS_REQUIRED plugin1 [plugin2 ...]])
```

Meaning of the parameters is same as for `build_plugin`.
