/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <functional>

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/Log.hpp"
#include "core/String.hpp"
#include "core/Map.hpp"
#include "core/UniquePtr.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"
#include "simulator/Library.hpp"
#include "simulator/Program.hpp"

#if ENABLE_RENDER
#include "render/Context.hpp"
#endif

#if ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulator;
class Configuration;

/* ************************************************************************ */

/**
 * @brief Type for step number.
 */
using StepNumber = unsigned long long;

/* ************************************************************************ */

/**
 * @brief Simulation class.
 */
class DLL_EXPORT Simulation
{


// Public Types
public:


    /// Loaded library container.
    using LibraryContainer = Map<String, UniquePtr<Library>>;

    /// Module container type.
    using ModuleContainer = Map<String, UniquePtr<Module>>;

    /// Object container type.
    using ObjectContainer = DynamicArray<UniquePtr<Object>>;

    /// Container type for programs.
    using ProgramContainer = Map<String, Program>;


// Public Ctors
public:


    /**
     * @brief Constructor.
     */
    Simulation() NOEXCEPT;


    /**
     * @brief Virtual destructor.
     */
    virtual ~Simulation();


// Public Accessors
public:


    /**
     * @brief Returns step number.
     *
     * @return
     */
    StepNumber getStepNumber() const NOEXCEPT
    {
        return m_stepNumber;
    }


    /**
     * @brief Returns a number of total iterations.
     *
     * @return
     */
    StepNumber getIterations() const NOEXCEPT
    {
        return m_iterations;
    }


    /**
     * @brief Returns if there is unlimited iterations.
     *
     * @return
     */
    bool hasUnlimitedIterations() const NOEXCEPT
    {
        return getIterations() == 0;
    }


    /**
     * @brief Return a list of modules.
     *
     * @return
     */
    const ModuleContainer& getModules() const NOEXCEPT
    {
        return m_modules;
    }


    /**
     * @brief Returns simulation time step.
     *
     * @return
     */
	units::Duration getTimeStep() const NOEXCEPT
    {
        return m_timeStep;
    }


    /**
     * @brief If real-time step is enabled.
     *
     * @return
     */
    bool isTimeStepRealTime() const NOEXCEPT
    {
        return m_timeStepRealTime;
    }


    /**
     * @brief Returns if library with given name is loaded.
     *
     * @param name
     *
     * @return
     */
    bool hasLibrary(const String& name) const NOEXCEPT
    {
        return m_libraries.find(name) != m_libraries.end();
    }


    /**
     * @brief If simulation uses given module.
     *
     * @param name Module name.
     *
     * @return
     */
    bool hasModule(const String& name) const NOEXCEPT
    {
        return m_modules.find(name) != m_modules.end();
    }


    /**
     * @brief Get module by name.
     *
     * @param name Module name.
     *
     * @return Pointer to module. If module doesn't exists, nullptr is returned.
     */
    Module* getModule(const String& name) NOEXCEPT
    {
        auto it = m_modules.find(name);

        if (it == m_modules.end())
            return nullptr;

        return it->second.get();
    }


    /**
     * @brief Find module by type.
     *
     * @tparam ModuleType Module type
     *
     * @param name Module name.
     *
     * @return Pointer to module. If module doesn't exists, nullptr is returned.
     */
    template<typename ModuleType>
    ModuleType* getModule(const String& name) NOEXCEPT
    {
        auto module = getModule(name);
        if (!module)
            return nullptr;

        assert(dynamic_cast<ModuleType*>(module));
        return static_cast<ModuleType*>(module);
    }


    /**
     * @brief Return a list of world objects.
     *
     * @return
     */
    const ObjectContainer& getObjects() const NOEXCEPT
    {
        return m_objects;
    }


    /**
     * @brief Returns world size.
     *
     * @return
     */
    const SizeVector& getWorldSize() const NOEXCEPT
    {
        return m_worldSize;
    }


#if ENABLE_PHYSICS
    /**
     * @brief Returns physics world.
     *
     * @return
     */
    b2World& getWorld() NOEXCEPT
    {
        return m_world;
    }
#endif


#if ENABLE_PHYSICS
    /**
     * @brief Returns physics world.
     *
     * @return
     */
    const b2World& getWorld() const NOEXCEPT
    {
        return m_world;
    }
#endif


#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    /**
     * @brief Returns if physics debug data is shown.
     *
     * @return
     */
    bool isDrawPhysics() const NOEXCEPT
    {
        return m_drawPhysics;
    }
#endif


    /**
     * @brief Returns a map of preddefined programs.
     *
     * @return
     */
    const ProgramContainer& getPrograms() const NOEXCEPT
    {
        return m_programs;
    }


    /**
     * @brief Check if program with given name exists.
     *
     * @param name Program name.
     *
     * @return
     */
    bool hasProgram(const String& name) const NOEXCEPT
    {
        return m_programs.find(name) != m_programs.end();
    }


    /**
     * @brief Returns program.
     *
     * @param name Program name.
     *
     * @return Pointer to program.
     */
    Program getProgram(const String& name) NOEXCEPT
    {
        auto it = m_programs.find(name);

        // Try to build program from module
        if (it == m_programs.end())
            return buildProgram(name);

        return it->second;
    }


// Public Mutators
public:


    /**
     * @brief Set total number of simulation iterations.
     *
     * @param iterations Number of iterations. If value is 0, there is
     *                   unlimited number of iterations.
     */
    void setIterations(StepNumber iterations) NOEXCEPT
    {
        m_iterations = iterations;
    }


    /**
     * @brief Set simulation time step.
     *
     * @param dt
     */
	void setTimeStep(units::Duration dt) NOEXCEPT
    {
        m_timeStep = dt;
    }


    /**
     * @brief Enable or disable real-time time step.
     *
     * @param flag
     */
    void setTimeStepRealTime(bool flag) NOEXCEPT
    {
        m_timeStepRealTime = flag;
    }


    /**
     * @brief Add new module.
     *
     * @param name Module name.
     * @param mod  Pointer to module.
     *
     * @return A pointer to inserted module.
     */
    template<typename T>
    T* addModule(String name, UniquePtr<T> mod)
    {
        assert(mod);
        auto it = m_modules.emplace(std::make_pair(std::move(name), std::move(mod)));
        return static_cast<T*>(std::get<0>(it)->second.get());
    }


    /**
     * @brief Create module.
     *
     * @param name Module name.
     * @param args...
     *
     * @return A pointer to created module.
     */
    template<typename T, typename... Args>
    T* createModule(String name, Args&&... args)
    {
        return addModule(std::move(name), makeUniquePtr<T>(std::forward<Args>(args)...));
    }


    /**
     * @brief Load module from library and add it into simulation.
     *
     * @param path Path to module.
     *
     * @return A pointer to created module.
     */
    Module* useModule(const String& path);


    /**
     * @brief Load module from library and add it into simulation.
     *
     * @tparam ModuleType
     *
     * @param path Path to module.
     *
     * @return A pointer to created module.
     */
    template<typename ModuleType>
    ModuleType* useModule(const String& path)
    {
        return static_cast<ModuleType*>(useModule(path));
    }


    /**
     * @brief Change world size.
     *
     * @param size
     */
	void setWorldSize(SizeVector size) NOEXCEPT
    {
        m_worldSize = std::move(size);
    }


    /**
     * @brief Add a new object to the world.
     *
     * @param obj
     */
    template<typename T>
    T* addObject(UniquePtr<T> obj)
    {
        assert(obj);
        m_objects.push_back(std::move(obj));
        return static_cast<T*>(m_objects.back().get());
    }


    /**
     * @brief Create a new object.
     *
     * @param args...
     *
     * @return
     */
    template<typename T, typename... Args>
    T* createObject(Args&&... args)
    {
        return addObject(makeUniquePtr<T>(*this, std::forward<Args>(args)...));
    }


    /**
     * @brief Build object by name.
     *
     * @param name    Object name.
     * @param dynamic Object is dynamic.
     *
     * @return
     */
    Object* buildObject(const String& name, bool dynamic = true);


#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    /**
     * @brief If physics debug data should be shown.
     */
    void setDrawPhysics(bool flag) NOEXCEPT
    {
        m_drawPhysics = flag;
    }
#endif


    /**
     * @brief Register new program.
     *
     * @param name    Program name.
     * @param program Added program.
     */
    void addProgram(String name, Program program)
    {
        m_programs.emplace(std::move(name), std::move(program));
    }


    /**
     * @brief Create a program from module.
     *
     * @param name Program name.
     *
     * @return Created program.
     */
    Program buildProgram(const String& path);


// Public Operations
public:


    /**
     * @brief Reset simulation.
     */
    void reset();


    /**
     * @brief Update simulation.
     *
     * @param dt
     *
     * @return If next step can be calculated.
     */
	bool update(units::Duration dt);


    /**
     * @brief Update simulation by time step.
     *
     * @return If next step can be calculated.
     */
    bool update();


#if ENABLE_RENDER
    /**
     * @brief Render simulation.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context);
#endif


    /**
     * @brief Load library into cache and return pointer.
     *
     * In case the library was loaded before, it't not loaded again.
     *
     * @param name Library name.
     *
     * @return
     */
    Library* loadLibrary(const String& name);


    /**
     * @brief Returns library API for required library.
     *
     * @param name  Library name.
     *
     * @return
     */
    LibraryApi* getLibraryApi(const String& name);


// Private Data Members
private:

    /// Number of simulation steps.
    StepNumber m_stepNumber = 0;

    /// Number of iterations.
    StepNumber m_iterations = 0;

    /// Simulation step.
	units::Duration m_timeStep;

    /// Real-time time step
    bool m_timeStepRealTime = true;

    /// World size.
	SizeVector m_worldSize{ units::um(400), units::um(400) };

    /// Cache for loaded libraries.
    LibraryContainer m_libraries;

    /// Simulation modules.
    ModuleContainer m_modules;

#ifdef ENABLE_PHYSICS
    b2World m_world;
#endif

    /// Simulation objects.
    ObjectContainer m_objects;

    /// A map of preddefined programs.
    ProgramContainer m_programs;

#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    bool m_drawPhysics = true;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
