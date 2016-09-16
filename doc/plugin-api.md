---
layout: article
permalink: /documentation/plugin-api/
title: Plugin API
share: false
categories:
    - documentation
---

Additional simulator functionality must be provided by plugins which are loaded dynamically. Plugin is a package of loaders, modules, object types and programs which can be used in simulations. When plugin is loaded, it register exported loader, module etc. so it can be shown what from plugin can be used.

The most important class for plugins is `cece::plugin::Api` which is used as communication interface between core library and plugin. Plugin has to extend this class and override important member functions.

> Library API is still not finished so following can be changed over time. Constant `cece::config::PLUGIN_API_VERSION` specify plugin API version and when the API is changed the number is also changed.

Class is defined in `cece/plugin/Api.hpp` and have following member functions that can be overriden.

* `DynamicArray<String> requiredPlugins() const` - a list of plugins that must be loaded before this plugin (and unloaded after).
* `DynamicArray<String> conflictPlugins() const` - a list of plugins that cannot be used with this plugin.
* `void onLoad(Repository& repository) const` - when plugin is loaded, used for offered objects registration.
* `void onUnload(Repository& repository) const` - when plugin is unloaded, registered object must be removed here.
* `void initSimulation(simulator::Simulation& simulation) const` - called when plugin is imported into simulation.
* `void finalizeSimulation(simulator::Simulation& simulation) const` - called when plugin is removed from simulation.
* `void loadConfig(simulator::Simulation& simulation, const config::Configuration& config) const` - called when plugin is being configured for specific simulation.
* `void storeConfig(const simulator::Simulation& simulation, config::Configuration& config) const` - called when configuration needs to be stored.

The plugin must export API in specific way which is understandable by the core library. This is handled by `CECE_DEFINE_PLUGIN` macro which is defined in `cece/plugin/definition.hpp`. Macro also adds checks to ensure the plugin match API version and other things that can be changed in configuration.
Following code shows minimum plugin definition.

```c++
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"

class DummyApi : public cece::plugin::Api { };

CECE_DEFINE_PLUGIN(dummy, DummyApi)
```
