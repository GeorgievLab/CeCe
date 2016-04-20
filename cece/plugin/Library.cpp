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

// CeCe Config
#include "cece/config.hpp"

/* ************************************************************************ */

// Declaration
#include "cece/plugin/Library.hpp"

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

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/plugin/definition.hpp"
#include "cece/plugin/Api.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

#if __linux__
const String Library::FILE_PREFIX = "libplugin-";
#elif _WIN32
const String Library::FILE_PREFIX = "libplugin-";
#elif __APPLE__ && __MACH__
const String Library::FILE_PREFIX = "libplugin-";
#endif

/* ************************************************************************ */

#if __linux__
const String Library::FILE_EXTENSION = ".so";
#elif _WIN32
const String Library::FILE_EXTENSION = ".dll";
#elif __APPLE__ && __MACH__
const String Library::FILE_EXTENSION = ".dylib";
#endif

/* ************************************************************************ */

/**
 * @brief OS dependent library implementation.
 */
struct Library::Impl
{
// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param path
     */
    explicit Impl(FilePath path)
        : m_path(std::move(path))
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
        // Get error message
        const auto err = ::GetLastError();
        if (err == 0)
            return {};

        LPSTR buffer = nullptr;
        const auto size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL
        );

        String message(buffer, size);

        // Free the buffer
        LocalFree(buffer);

        return message;
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

Library::Library(String name, UniquePtr<Api> api)
    : m_name(std::move(name))
    , m_api(std::move(api))
{
    // Nothing to do
}

/* ************************************************************************ */

Library::Library(String name, FilePath path)
    : m_name(std::move(name))
{
    // Create dynamic implementation
    m_impl.reset(new Impl{std::move(path)});

    if (!m_impl->isLoaded())
        throw RuntimeException("Library '" + getName() + "' cannot be loaded: " + m_impl->getError());

    // Get configuration
    auto getConfigFn = m_impl->getAddr<GetConfigFn>("get_config");

    if (!getConfigFn)
        throw RuntimeException("Library '" + getName() + "' doesn't contains 'get_config' function");

    // Check configuration
    checkConfig(getConfigFn());

    // Get create API function
    auto fn = m_impl->getAddr<CreateFn>("create");

    if (!fn)
        throw RuntimeException("Library '" + getName() + "' doesn't contains 'create' function");

    Log::debug("Library loaded `", getName(), "`.");

    // Create extension object
    m_api.reset(fn());
}

/* ************************************************************************ */

Library::~Library()
{
    Log::debug("Library released `", getName(), "`.");
}

/* ************************************************************************ */

void Library::checkConfig(Config* config)
{
    if (!config)
        throw RuntimeException("Library '" + getName() + "' returns no config");

    if (config->apiVersion != config::PLUGIN_API_VERSION)
        throw RuntimeException("Library '" + getName() + "' is built against different API version than CeCe");

    if (config->dimension != config::DIMENSION)
        throw RuntimeException("Library '" + getName() + "' returns nullptr config");

    if (config->realSize != sizeof(config::RealType))
        throw RuntimeException("Library '" + getName() + "' is built with different real type than CeCe");

#ifdef CECE_ENABLE_RENDER
    if (!config->renderEnabled)
        throw RuntimeException("Library '" + getName() + "' is built without render support, but CeCe did");
#else
    if (config->renderEnabled)
        throw RuntimeException("Library '" + getName() + "' is built with render support, but CeCe didn't");
#endif

#ifdef CECE_ENABLE_BOX2D_PHYSICS
    if (!config->builtinPhysics)
        throw RuntimeException("Library '" + getName() + "' is built without builtin physics, but CeCe did");
#else
    if (config->builtinPhysics)
        throw RuntimeException("Library '" + getName() + "' is built with builtin physics, but CeCe didn't");
#endif

#ifdef CECE_THREAD_SAFE
    if (!config->threadSafe)
        throw RuntimeException("Library '" + getName() + "' is built without thread safety, but CeCe did");
#else
    if (config->threadSafe)
        throw RuntimeException("Library '" + getName() + "' is built with thread safety, but CeCe didn't");
#endif
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
