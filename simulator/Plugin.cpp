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

DynamicArray<String> Plugin::s_directories{
    DIR_PLUGINS
};

/* ************************************************************************ */

#define ITEM(name, validname) extern "C" simulator::PluginApi* PLUGIN_PROTOTYPE_NAME_BUILTIN(create, validname)();
BUILTIN_PLUGINS
#undef ITEM

/* ************************************************************************ */

#define ITEM(name, validname) { # name, PLUGIN_PROTOTYPE_NAME_BUILTIN(create, validname) },
const Map<String, Plugin::CreateFn> Plugin::s_builtinPlugins{
    BUILTIN_PLUGINS
};
#undef ITEM

/* ************************************************************************ */

Map<String, FilePath> Plugin::s_externPlugins = Plugin::scanDirectories();

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
        m_ptr = LoadLibrary(m_path.c_str());
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
    FilePath m_path;

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
    auto it = s_builtinPlugins.find(getName());

    // Required library is builtin
    if (it != s_builtinPlugins.end())
    {
        Log::debug("Loading builtin plugin `", getName(), "`...");

        // Create library API
        m_api.reset(it->second());
    }
    else
    {
        Log::debug("Loading external plugin `", getName(), "`...");

        auto it = s_externPlugins.find(getName());

        if (it == s_externPlugins.end())
            throw RuntimeException("Plugin cannot be found");

        // Create dynamic implementation
        m_impl.reset(new Impl{it->second});

        if (!m_impl->isLoaded())
            throw RuntimeException("Plugin is not loaded: " + m_impl->getError());

        // Check API version
        auto apiVerFn = m_impl->getAddr<ApiVersionFn>("api_version");

        if (!apiVerFn)
            throw RuntimeException("Plugin doesn't contains 'api_version' function");

        if (apiVerFn() != PLUGIN_API_VERSION)
            throw RuntimeException("Plugin API version is different from the simulator");

        auto fn = m_impl->getAddr<CreateFn>("create");

        if (!fn)
            throw RuntimeException("Plugin doesn't contains 'create' function");

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

void Plugin::addDirectory(String path)
{
    Log::debug("New plugins directory: `", path, "`");

    s_directories.push_back(std::move(path));
}

/* ************************************************************************ */

DynamicArray<String> Plugin::getNamesBuiltin() NOEXCEPT
{
    DynamicArray<String> names;
    names.reserve(s_builtinPlugins.size());

    for (const auto& p : s_builtinPlugins)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

DynamicArray<String> Plugin::getNamesExtern() NOEXCEPT
{
    DynamicArray<String> names;
    names.reserve(s_externPlugins.size());

    for (const auto& p : s_externPlugins)
        names.push_back(p.first);

    return names;
}

/* ************************************************************************ */

DynamicArray<String> Plugin::getNames() NOEXCEPT
{
    auto names = getNamesBuiltin();
    auto namesExtern = getNamesExtern();
    names.reserve(names.size() + namesExtern.size());

    std::move(namesExtern.begin(), namesExtern.end(), std::back_inserter(names));

    return names;
}

/* ************************************************************************ */

Map<String, FilePath> Plugin::scanDirectory(const FilePath& directory) NOEXCEPT
{
    using namespace boost::filesystem;

    Map<String, FilePath> result;

    // Regular expression
    const String pattern = g_prefix + "(.*)" + g_extension;
    std::regex regex(pattern);
    std::smatch matches;

    Log::debug("Scanning `", directory.string(), "` for plugins with pattern `", pattern, "`");

    if (!is_directory(directory))
    {
        Log::warning("Directory `", directory.string(), "` doesn't exists");
        return result;
    }

    // Foreach directory
    for (const auto& entry : makeRange(directory_iterator(directory), directory_iterator()))
    {
        // Only files
        if (!is_regular_file(entry))
            continue;

        // Get path
        auto path = entry.path();
        const auto filename = path.filename();

        // Match file name
        if (!std::regex_match(filename.string(), matches, regex))
            continue;

        if (matches.size() == 2)
            result.emplace(matches[1].str(), std::move(path));
    }

    return result;
}

/* ************************************************************************ */

Map<String, FilePath> Plugin::scanDirectories() NOEXCEPT
{
    using namespace boost::filesystem;

    Map<String, FilePath> result;

    // Foreach directories
    for (const auto& dirName : getDirectories())
    {
        auto tmp = scanDirectory(dirName);
        result.insert(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()));
    }

    return result;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
