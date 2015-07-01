/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

// Declaration
#include "Plugin.hpp"

// C++
#include <cstdint>
#include <cstdlib>

#if __linux__
#include <dlfcn.h>
#elif _WIN32
#include <windows.h>
#elif __APPLE__ && __MACH__
#include <dlfcn.h>
#else
#error Unsupported platform
#endif

// Simulator
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "core/FilePath.hpp"
#include "simulator/PluginApi.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#if __linux__
const String g_prefix = "libplugin-";
#elif _WIN32
const String g_prefix = "libplugin-";
#elif __APPLE__ && __MACH__
const String g_prefix = "libplugin-";
#endif

/* ************************************************************************ */

#if __linux__
const String g_extension = ".so";
#elif _WIN32
const String g_extension = ".dll";
#elif __APPLE__ && __MACH__
const String g_extension = ".dylib";
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

DynamicArray<String> Plugin::s_libraryPaths;

/* ************************************************************************ */

#define ITEM(name, validname) extern "C" simulator::PluginApi* PLUGIN_PROTOTYPE_NAME_BUILTIN(create, validname)();
BUILTIN_PLUGINS
#undef ITEM

/* ************************************************************************ */

#define ITEM(name, validname) { # name, PLUGIN_PROTOTYPE_NAME_BUILTIN(create, validname) },
const Map<String, Plugin::CreateFn> Plugin::s_builtinLibraries{
    BUILTIN_PLUGINS
};
#undef ITEM

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
     * @param name
     */
    explicit Impl(const String& name)
        : m_filename(g_prefix + name + g_extension)
    {
        Log::debug("Loading shared library: ", m_filename);
#if __linux__ || __APPLE__ && __MACH__
        m_ptr = dlopen(m_filename.c_str(), RTLD_LAZY);
#elif _WIN32
        m_ptr = LoadLibrary(m_filename.c_str());
#endif
    }


    /**
     * @brief Destructor.
     */
    ~Impl()
    {
        Log::debug("Closing shared library: ", m_filename);
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
    bool isLoaded() const NOEXCEPT
    {
        return m_ptr != nullptr;
    }


    /**
     * @brief Returns error string.
     *
     * @return
     */
    String getError() const NOEXCEPT
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
    void* getAddr(const char* name) const NOEXCEPT
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
    T getAddr(const char* name) const NOEXCEPT
    {
        return reinterpret_cast<T>(reinterpret_cast<std::intptr_t>(getAddr(name)));
    }


// Private Data Members
private:

    /// Source file name.
    FilePath m_filename;

#if __linux__ || __APPLE__ && __MACH__
    void* m_ptr;
#elif _WIN32
    HMODULE m_ptr;
#endif

};

/* ************************************************************************ */

Plugin::Plugin(String name)
    : m_name(std::move(name))
{
    // Try to find in builtin libraries
    auto it = s_builtinLibraries.find(getName());

    // Required library is builtin
    if (it != s_builtinLibraries.end())
    {
        Log::debug("Loading builtin plugin `", getName(), "`...");

        // Create library API
        m_api.reset(it->second());
    }
    else
    {
        Log::debug("Loading external plugin `", getName(), "`...");

        // Create dynamic implementation
        m_impl.reset(new Impl{getName()});

        if (!m_impl->isLoaded())
            throw RuntimeException("Library is not loaded: " + m_impl->getError());

        // Check API version
        auto apiVerFn = m_impl->getAddr<ApiVersionFn>("api_version");

        if (!apiVerFn)
            throw RuntimeException("Library doesn't contains 'api_version' function");

        if (apiVerFn() != PLUGIN_API_VERSION)
            throw RuntimeException("Library API version is different from the simulator");

        auto fn = m_impl->getAddr<CreateFn>("create");

        if (!fn)
            throw RuntimeException("Library doesn't contains 'create' function");

        // Create extension object
        m_api.reset(fn());
    }

    Log::debug("Done");
}

/* ************************************************************************ */

Plugin::~Plugin()
{
    Log::debug("Plugin released `", getName(), "`");
}

/* ************************************************************************ */

void Plugin::addLibraryPath(String path)
{
#if __linux__ || __APPLE__ && __MACH__
    // Get previous paths
    String paths;
    char* p = getenv("LD_LIBRARY_PATH");

    // Previously set
    if (p)
    {
        paths = p;
        paths.push_back(':');
    }

    // Append new path
    paths.append(path);

    // Update environment value
    setenv("LD_LIBRARY_PATH", paths.c_str(), 1);
#endif

    s_libraryPaths.push_back(std::move(path));
}

/* ************************************************************************ */

DynamicArray<String> Plugin::getBuiltInNames() NOEXCEPT
{
    DynamicArray<String> names;
    names.reserve(s_builtinLibraries.size());

    for (const auto& p : s_builtinLibraries)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

DynamicArray<String> getExternNames() NOEXCEPT
{
    DynamicArray<String> names;

    // TODO: detect available libraries

    return names;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
