
// Declaration
#include "Library.hpp"

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
#include "simulator/LibraryApi.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#if __linux__
const String g_prefix = "libmodule-";
#elif _WIN32
const String g_prefix = "libmodule-";
#elif __APPLE__ && __MACH__
const String g_prefix = "libmodule-";
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

DynamicArray<String> Library::s_libraryPaths;

/* ************************************************************************ */

#define ITEM(name, validname) extern "C" simulator::LibraryApi* LIBRARY_PROTOTYPE_NAME_BUILDIN(create, validname)();
BUILDIN_LIBRARIES
#undef ITEM

/* ************************************************************************ */

#define ITEM(name, validname) { # name, LIBRARY_PROTOTYPE_NAME_BUILDIN(create, validname) },
const Map<String, Library::CreateFn> Library::s_buildinLibraries{
    BUILDIN_LIBRARIES
};
#undef ITEM

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

Library::Library(const String& name)
{
    // Try to find in build-in libraries
    auto it = s_buildinLibraries.find(name);

    // Required library is build-in
    if (it != s_buildinLibraries.end())
    {
        // Create library API
        m_api.reset(it->second());
    }
    else
    {
        // Create dynamic implementation
        m_impl.reset(new Impl{name});

        if (!m_impl->isLoaded())
            throw RuntimeException("Library is not loaded");

        // Check API version
        auto apiVerFn = m_impl->getAddr<ApiVersionFn>("api_version");

        if (!apiVerFn)
            throw RuntimeException("Library doesn't contains 'api_version' function");

        if (apiVerFn() != LIBRARY_API_VERSION)
            throw RuntimeException("Library API version is different from the simulator");

        auto fn = m_impl->getAddr<CreateFn>("create");

        if (!fn)
            throw RuntimeException("Library doesn't contains 'create' function");

        // Create extension object
        m_api.reset(fn());
    }
}

/* ************************************************************************ */

Library::~Library()
{
    // Nothing to do
}

/* ************************************************************************ */

bool Library::isLoaded() const NOEXCEPT
{
    if (!m_impl)
        return true;

    return m_impl->isLoaded();
}
/* ************************************************************************ */

String Library::getError() const NOEXCEPT
{
    if (!m_impl)
        return {};

    return m_impl->getError();
}

/* ************************************************************************ */

void Library::addLibraryPath(String path)
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

DynamicArray<String> Library::getBuildInNames() NOEXCEPT
{
    DynamicArray<String> names;
    names.reserve(s_buildinLibraries.size());

    for (const auto& p : s_buildinLibraries)
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
