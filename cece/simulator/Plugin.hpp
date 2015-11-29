/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/String.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/simulator/Program.hpp"
#include "cece/simulator/Module.hpp"
#include "cece/simulator/Object.hpp"

/* ************************************************************************ */

/**
 * @brief Plugin API version.
 */
#define PLUGIN_API_VERSION 2

/* ************************************************************************ */

/**
 * @brief Define function name for extern plugins.
 *
 * @param prefix Function prefix.
 * @param name   Plugin name.
 */
#define PLUGIN_PROTOTYPE_NAME_EXTERN(prefix, name) prefix

/* ************************************************************************ */

/**
 * @brief Define function name for builtin plugins.
 *
 * @param prefix Function prefix.
 * @param name   Plugin name.
 */
#define PLUGIN_PROTOTYPE_NAME_BUILTIN(prefix, name) prefix ## _ ## name

/* ************************************************************************ */

/**
 * @brief Function name.
 *
 * @param name Plugin name.
 */
#if PLUGIN_BUILTIN
#define PLUGIN_PROTOTYPE_NAME(prefix, name) PLUGIN_PROTOTYPE_NAME_BUILTIN(prefix, name)
#else
#define PLUGIN_PROTOTYPE_NAME(prefix, name) PLUGIN_PROTOTYPE_NAME_EXTERN(prefix, name)
#endif

/* ************************************************************************ */

/**
 * @brief Prototype of function for creating plugin API object.
 *
 * @param name Plugin name.
 */
#define PLUGIN_CREATE_PROTOTYPE(name) \
    extern "C" cece::simulator::PluginApi* PLUGIN_PROTOTYPE_NAME(create, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for creating plugin API object.
 *
 * @param name Plugin name.
 */
#define DECLARE_PLUGIN_CREATE(name) PLUGIN_CREATE_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for creating plugin API object.
 *
 * @param name      Plugin name.
 * @param className Plugin API class name.
 */
#define DEFINE_PLUGIN_CREATE(name, className) \
    PLUGIN_CREATE_PROTOTYPE(name) \
    { \
        return new className{}; \
    }

/* ************************************************************************ */

/**
 * @brief Prototype of function for returning plugin API version.
 *
 * @param name Plugin name.
 */
#define PLUGIN_API_VERSION_PROTOTYPE(name) \
    extern "C" int PLUGIN_PROTOTYPE_NAME(api_version, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for returning plugin API version.
 *
 * @param name Plugin name.
 */
#define DECLARE_PLUGIN_API_VERSION(name) PLUGIN_API_VERSION_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for returning plugin API version.
 *
 * @param name Plugin name.
 */
#define DEFINE_PLUGIN_API_VERSION(name) \
    PLUGIN_API_VERSION_PROTOTYPE(name) \
    { \
        return PLUGIN_API_VERSION; \
    }

/* ************************************************************************ */

/**
 * @brief Prototype of function for returning plugin real type size.
 *
 * @param name Plugin name.
 */
#define PLUGIN_REAL_SIZE_PROTOTYPE(name) \
    extern "C" unsigned int PLUGIN_PROTOTYPE_NAME(real_size, name)()

/* ************************************************************************ */

/**
 * @brief Declare function for returning plugin real type size.
 *
 * @param name Plugin name.
 */
#define DECLARE_PLUGIN_REAL_SIZE(name) PLUGIN_REAL_SIZE_PROTOTYPE(name)

/* ************************************************************************ */

/**
 * @brief Define function for returning plugin real type size.
 *
 * @param name Plugin name.
 */
#define DEFINE_PLUGIN_REAL_SIZE(name) \
    PLUGIN_REAL_SIZE_PROTOTYPE(name) \
    { \
        return sizeof(RealType); \
    }

/* ************************************************************************ */

/**
 * @brief Declare plugin functions.
 *
 * @param name Plugin name.
 */
#define DECLARE_PLUGIN(name) \
    DECLARE_PLUGIN_CREATE(name); \
    DECLARE_PLUGIN_API_VERSION(name); \
    DECLARE_PLUGIN_REAL_SIZE(name)

/* ************************************************************************ */

/**
 * @brief Define plugin functions.
 *
 * @param name      Plugin name.
 * @param className Plugin API class name.
 */
#define DEFINE_PLUGIN(name, className) \
    DEFINE_PLUGIN_CREATE(name, className) \
    DEFINE_PLUGIN_API_VERSION(name) \
    DEFINE_PLUGIN_REAL_SIZE(name)

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

class PluginApi;
class PluginContext;
class Simulation;

/* ************************************************************************ */

/**
 * @brief Helper class for wrapping plugin.
 */
class Plugin final
{

// Public Constants
public:


    /// Plugin file prefix
    static const String FILE_PREFIX;

    /// Plugin file extension
    static const String FILE_EXTENSION;


// Public Types
public:


    /// Create API function pointer type.
    using CreateFn = PluginApi* (*)();

    /// API version function pointer type.
    using ApiVersionFn = int (*)();

    /// Size of real type.
    using RealSizeFn = unsigned int (*)();


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name    Plugin name.
     * @param api     Plugin builtin api.
     */
    Plugin(String name, UniquePtr<PluginApi> api);


    /**
     * @brief Constructor.
     *
     * @param name    Plugin name.
     * @param path    Path to plugin.
     */
    Plugin(String name, FilePath path);


    /**
     * @brief Destructor.
     */
    ~Plugin();


// Public Accessors
public:


    /**
     * @brief Returns plugin name.
     *
     * @return
     */
    const String& getName() const noexcept
    {
        return m_name;
    }


    /**
     * @brief Returns API object.
     *
     * @return
     */
    ViewPtr<PluginApi> getApi() const noexcept
    {
        return m_api;
    }


// Private Data Members
private:

    /// Plugin name.
    String m_name;

    /// Implementation
    struct Impl;
    UniquePtr<Impl> m_impl;

    /// Plugin API.
    UniquePtr<PluginApi> m_api;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
