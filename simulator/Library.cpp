
// Declaration
#include "Library.hpp"

// C++
#include <map>
#include <cstdint>

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

namespace {

/* ************************************************************************ */

#if __linux__
const std::string g_extension = ".so";
#elif __WIN32__
const std::string g_extension = ".dll";
#endif

/* ************************************************************************ */

}

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

    /**
     * @brief Returns address of required symbol.
     *
     * @param name
     */
    void* getAddr(const char* name) const noexcept
    {
#if __linux__
        return dlsym(lib, name);
#elif __WIN32__
        return GetProcAddress(lib, name);
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
};

/* ************************************************************************ */

Library::Library(const std::string& name)
    : m_impl{new Impl{nullptr}}
{
    const std::string filename = name + g_extension;
#if __linux__
    m_impl->lib = dlopen(filename.c_str(), RTLD_LAZY);
#elif __WIN32__
    m_impl->lib = LoadLibrary(filename.c_str());
#endif

    // Set create function
    m_createModule = m_impl->getAddr<CreateModule>("create_module");
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
    return std::unique_ptr<Module>{m_createModule(name.c_str())};
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
