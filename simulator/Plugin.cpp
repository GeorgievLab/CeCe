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

// Declaration
#include "Plugin.hpp"

// C++
#include <cstdint>
#include <cstdlib>
#include <regex>

// Boost
#include <boost/filesystem.hpp>

#if __linux__ || __APPLE__ && __MACH__
#include <dlfcn.h>
#elif _WIN32
#include <windows.h>
#else
#error Unsupported platform
#endif

// Simulator
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "core/FilePath.hpp"
#include "core/Range.hpp"
#include "simulator/PluginApi.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

#if __linux__
const String Plugin::FILE_PREFIX = "libplugin-";
#elif _WIN32
const String Plugin::FILE_PREFIX = "libplugin-";
#elif __APPLE__ && __MACH__
const String Plugin::FILE_PREFIX = "libplugin-";
#endif

/* ************************************************************************ */

#if __linux__
const String Plugin::FILE_EXTENSION = ".so";
#elif _WIN32
const String Plugin::FILE_EXTENSION = ".dll";
#elif __APPLE__ && __MACH__
const String Plugin::FILE_EXTENSION = ".dylib";
#endif

/* ************************************************************************ */

/**
 * @brief OS dependent library implementation.
 */
struct Plugin::Impl
{
// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param path
     */
    explicit Impl(const FilePath& path)
        : m_path(path)
    {
        Log::debug("Loading shared library: ", m_path);
#if __linux__ || __APPLE__ && __MACH__
        m_ptr = dlopen(m_path.c_str(), RTLD_LAZY);
#elif _WIN32
        m_ptr = LoadLibraryW(m_path.c_str());
#endif
    }


    /**
     * @brief Destructor.
     */
    ~Impl()
    {
        Log::debug("Closing shared library: ", m_path);
#if __linux__ || __APPLE__ && __MACH__
        if (m_ptr)
            dlclose(m_ptr);
#elif _WIN32
        FreeLibrary(m_ptr);
#endif
    }


// Public Accessors
public:


    /**
     * @brief Returns if library is loaded.
     *
     * @return
     */
    bool isLoaded() const noexcept
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Returns error string.
     *
     * @return
     */
    String getError() const noexcept
    {
#if __linux__ || __APPLE__ && __MACH__
        return dlerror();
#elif _WIN32
        return "Error code: " + std::to_string(GetLastError());
#endif
    }


    /**
     * @brief Returns address of required symbol.
     *
     * @param name
     */
    void* getAddr(const char* name) const noexcept
    {
#if __linux__ || __APPLE__ && __MACH__
        return dlsym(m_ptr, name);
#elif _WIN32
        return reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(GetProcAddress(m_ptr, name)));
#endif
    }


    /**
     * @brief Returns address of required symbol.
     *
     * @param name
     */
    template<typename T>
    T getAddr(const char* name) const noexcept
    {
        return reinterpret_cast<T>(reinterpret_cast<std::intptr_t>(getAddr(name)));
    }


// Private Data Members
private:

    /// Source file name.
    FilePath m_path;

#if __linux__ || __APPLE__ && __MACH__
    void* m_ptr;
#elif _WIN32
    HMODULE m_ptr;
#endif

};

/* ************************************************************************ */

Plugin::Plugin(String name, UniquePtr<PluginApi> api)
    : m_name(std::move(name))
    , m_api(std::move(api))
{
    // Nothing to do
}

/* ************************************************************************ */

Plugin::Plugin(String name, FilePath path)
    : m_name(std::move(name))
{
    // Create dynamic implementation
    m_impl.reset(new Impl{std::move(path)});

    if (!m_impl->isLoaded())
        throw RuntimeException("Plugin is not loaded: " + m_impl->getError());

    // Check API version
    auto apiVerFn = m_impl->getAddr<ApiVersionFn>("api_version");

    if (!apiVerFn)
        throw RuntimeException("Plugin doesn't contains 'api_version' function");

    if (apiVerFn() != PLUGIN_API_VERSION)
        throw RuntimeException("Plugin API version is different from the simulator");

    // Check real type size
    auto realSizeFn = m_impl->getAddr<RealSizeFn>("real_size");

    if (!realSizeFn)
        throw RuntimeException("Plugin doesn't contains 'real_size' function");

    if (realSizeFn() != sizeof(RealType))
        throw RuntimeException("Plugin real type size is different from the simulator");

    auto fn = m_impl->getAddr<CreateFn>("create");

    if (!fn)
        throw RuntimeException("Plugin doesn't contains 'create' function");

    // Create extension object
    m_api.reset(fn());
}

/* ************************************************************************ */

Plugin::~Plugin()
{
    Log::debug("Plugin released `", getName(), "`");
}

/* ************************************************************************ */

}

/* ************************************************************************ */
