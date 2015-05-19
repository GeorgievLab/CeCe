
// Declaration
#include "Library.hpp"

// C++
#include <map>
#include <cstdint>
#include <cassert>
#include <cstdlib>

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
const std::string g_prefix = "libmodule-";
#elif __WIN32__
const std::string g_prefix = "libmodule-";
#endif

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

std::vector<std::string> Library::s_libraryPaths;

/* ************************************************************************ */

struct Library::Impl
{
#if __linux__
    void* lib;
#elif __WIN32__
    HMODULE* lib;
#endif

    /// Error string
    std::string error;


    /**
     * @brief Returns if library is loaded.
     *
     * @return
     */
    bool isLoaded() const noexcept
    {
#if __linux__
        return lib != nullptr;
#elif __WIN32__
        return lib != nullptr;
#endif
    }

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
    , m_createModule(nullptr)
{
    const std::string filename = g_prefix + name + g_extension;
#if __linux__
    m_impl->lib = dlopen(filename.c_str(), RTLD_LAZY);
    if (!m_impl->lib)
        m_impl->error = dlerror();
#elif __WIN32__
    m_impl->lib = LoadLibrary(filename.c_str());
#endif

    // Set create function
    if (m_impl->isLoaded())
    {
        m_initSimulation = m_impl->getAddr<InitSimulationFn>("init_simulation");
        m_finalizeSimulation = m_impl->getAddr<FinalizeSimulationFn>("finalize_simulation");
        m_createModule = m_impl->getAddr<CreateModuleFn>("create_module");
    }
}

/* ************************************************************************ */

Library::~Library()
{
#if __linux__
    if (m_impl->lib)
        dlclose(m_impl->lib);
#elif __WIN32__
    FreeLibrary(m_impl->lib);
#endif
}

/* ************************************************************************ */

bool Library::isLoaded() const noexcept
{
    return m_impl->isLoaded();
}

/* ************************************************************************ */

const std::string& Library::getError() const noexcept
{
    return m_impl->error;
}

/* ************************************************************************ */

void Library::addLibraryPath(std::string path)
{
#if __linux__
    // Get previous paths
    std::string paths;
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

void Library::initSimulation(Simulation* simulation)
{
    if (m_initSimulation)
        m_initSimulation(simulation);
}

/* ************************************************************************ */

void Library::finalizeSimulation(Simulation* simulation)
{
    if (m_finalizeSimulation)
        m_finalizeSimulation(simulation);
}

/* ************************************************************************ */

std::unique_ptr<Module> Library::createModule(Simulation* simulation, const std::string& name)
{
    assert(m_createModule);
    return std::unique_ptr<Module>{m_createModule(simulation, name.c_str())};
}

/* ************************************************************************ */

}

/* ************************************************************************ */
