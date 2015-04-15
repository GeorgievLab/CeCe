
// Declaration
#include "Library.hpp"

// C++
#include <map>

#if __linux__
// Linux
#include <dlfcn.h>
#elif __WIN32__
#include <windows.h>
#else
#error Unsupported platform
#endif

// Simulator
#include "Module.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

struct Library::Impl
{
#if __linux__
    void* lib;
#elif __WIN32__
    HMODULE* lib;
#endif
};

/* ************************************************************************ */

Library::Library(const std::string& name)
    : m_impl{new Impl{nullptr}}
{
#if __linux__
    const std::string filename = name + ".so";
    m_impl->lib = dlopen(filename.c_str(), RTLD_LAZY);
#elif __WIN32__
    const std::string filename = name + ".dll";
    m_impl->lib = LoadLibrary(filename.c_str());
#endif
}

/* ************************************************************************ */

Library::~Library()
{
#if __linux__
    dlclose(m_impl->lib);
#elif __WIN32__
    FreeLibrary(m_impl->lib);
#endif
}

/* ************************************************************************ */

bool Library::isLoaded() const noexcept
{
#if __linux__
    return m_impl->lib != nullptr;
#elif __WIN32__
    return m_impl->lib != nullptr;
#endif
}

/* ************************************************************************ */

std::unique_ptr<Module> Library::createModule(const std::string& name)
{
    return nullptr;
}

/* ************************************************************************ */

Library* Library::load(const std::string& name)
{
    static std::map<std::string, std::unique_ptr<Library>> s_cache;

    // Try to find library in cache
    auto it = s_cache.find(name);

    // Not found
    if (it == s_cache.end())
    {
        // Insert into cache
        auto ptr = s_cache.emplace(std::make_pair(
            name,
            std::unique_ptr<Library>{new Library(name)}
        ));
        it = std::get<0>(ptr);
    }

    // Return pointer
    return std::get<1>(*it).get();
}

/* ************************************************************************ */

std::unique_ptr<Module> Library::createModule(const std::string& library, const std::string& name)
{
    // Load library
    Library* lib = load(library);

    // Unable to load library
    if (lib == nullptr)
    {
        // TODO: print warning
        return nullptr;
    }

    // Create module
    return lib->createModule(name);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
