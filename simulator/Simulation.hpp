
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <vector>
#include <map>
#include <cassert>
#include <string>
#include <functional>

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/Log.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"
#include "simulator/Library.hpp"
#include "simulator/Program.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

#ifdef ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulator;
class ConfigurationBase;

/* ************************************************************************ */

/**
 * @brief Type for step number.
 */
using StepNumber = unsigned long long;

/* ************************************************************************ */

/**
 * @brief Simulation class.
 */
class Simulation
{


// Public Types
public:


    /// Module container type.
    using ModuleContainer = std::map<std::string, std::unique_ptr<Module>>;

    /// Object container type.
    using ObjectContainer = std::vector<std::unique_ptr<Object>>;


// Public Ctors
public:


    /**
     * @brief Constructor.
     *
     * @param simulator
     */
    explicit Simulation(Simulator& simulator) noexcept;


    /**
     * @brief Virtual destructor.
     */
    virtual ~Simulation();


// Public Accessors
public:


    /**
     * @brief Returns owning simulator.
     *
     * @return
     */
    Simulator& getSimulator() const noexcept
    {
        return m_simulator;
    }


    /**
     * @brief Returns step number.
     *
     * @return
     */
    StepNumber getStepNumber() const noexcept
    {
        return m_stepNumber;
    }


    /**
     * @brief Returns a number of total iterations.
     *
     * @return
     */
    StepNumber getIterations() const noexcept
    {
        return m_iterations;
    }


    /**
     * @brief Returns if there is unlimited iterations.
     *
     * @return
     */
    bool hasUnlimitedIterations() const noexcept
    {
        return getIterations() == 0;
    }


    /**
     * @brief Return a list of modules.
     *
     * @return
     */
    const ModuleContainer& getModules() const noexcept
    {
        return m_modules;
    }


    /**
     * @brief Returns simulation time step.
     *
     * @return
     */
    units::Duration getTimeStep() const noexcept
    {
        return m_timeStep;
    }


    /**
     * @brief If real-time step is enabled.
     *
     * @return
     */
    bool isTimeStepRealTime() const noexcept
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
    bool hasLibrary(const std::string& name) const noexcept
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
    bool hasModule(const std::string& name) const noexcept
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
    Module* getModule(const std::string& name) noexcept
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
    ModuleType* getModule(const std::string& name) noexcept
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
    const ObjectContainer& getObjects() const noexcept
    {
        return m_objects;
    }


    /**
     * @brief Returns world size.
     *
     * @return
     */
    const Vector<units::Length>& getWorldSize() const noexcept
    {
        return m_worldSize;
    }


#ifdef ENABLE_PHYSICS
    /**
     * @brief Returns physics world.
     *
     * @return
     */
    b2World& getWorld() noexcept
    {
        return m_world;
    }
#endif


#ifdef ENABLE_PHYSICS
    /**
     * @brief Returns physics world.
     *
     * @return
     */
    const b2World& getWorld() const noexcept
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
    bool isDrawPhysics() const noexcept
    {
        return m_drawPhysics;
    }
#endif


    /**
     * @brief Returns a map of preddefined programs.
     *
     * @return
     */
    const std::map<std::string, Program>& getPrograms() const noexcept
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
    bool hasProgram(const std::string& name) const noexcept
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
    Program getProgram(const std::string& name) noexcept
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
    void setIterations(StepNumber iterations) noexcept
    {
        m_iterations = iterations;
    }


    /**
     * @brief Set simulation time step.
     *
     * @param dt
     */
    void setTimeStep(units::Duration dt) noexcept
    {
        m_timeStep = dt;
    }


    /**
     * @brief Enable or disable real-time time step.
     *
     * @param flag
     */
    void setTimeStepRealTime(bool flag) noexcept
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
    T* addModule(std::string name, std::unique_ptr<T> mod)
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
    T* createModule(std::string name, Args&&... args)
    {
        return addModule(std::move(name), std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
    }


    /**
     * @brief Load module from library and add it into simulation.
     *
     * @param path Path to module.
     *
     * @return A pointer to created module.
     */
    Module* useModule(const std::string& path);


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
    ModuleType* useModule(const std::string& path)
    {
        return static_cast<ModuleType*>(useModule(path));
    }


    /**
     * @brief Change world size.
     *
     * @param size
     */
    void setWorldSize(Vector<units::Length> size) noexcept
    {
        m_worldSize = std::move(size);
    }


    /**
     * @brief Change world size.
     *
     * @param width  New width.
     * @param height New height.
     */
    void setWorldSize(units::Length width, units::Length height) noexcept
    {
        setWorldSize({width, height});
    }


    /**
     * @brief Add a new object to the world.
     *
     * @param obj
     */
    template<typename T>
    T* addObject(std::unique_ptr<T> obj)
    {
        assert(obj);
        m_objects.push_back(std::move(obj));
        T* ptr = static_cast<T*>(m_objects.back().get());
#ifdef ENABLE_RENDER
        m_drawInitList.push_back(ptr);
#endif
        return ptr;
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
        return addObject(std::unique_ptr<T>(new T(*this, std::forward<Args>(args)...)));
    }


    /**
     * @brief Build object by name.
     *
     * @param name    Object name.
     * @param dynamic Object is dynamic.
     *
     * @return
     */
    Object* buildObject(const std::string& name, bool dynamic = true);


#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    /**
     * @brief If physics debug data should be shown.
     */
    void setDrawPhysics(bool flag) noexcept
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
    void addProgram(std::string name, Program program)
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
    Program buildProgram(const std::string& path);


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
    bool update(units::Duration dt) noexcept;


    /**
     * @brief Update simulation by time step.
     *
     * @return If next step can be calculated.
     */
    bool update();


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize simulation for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context);
#endif


#ifdef ENABLE_RENDER
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
    Library* loadLibrary(const std::string& name);


    /**
     * @brief Returns library API for required library.
     *
     * @param name  Library name.
     *
     * @return
     */
    LibraryApi* getLibraryApi(const std::string& name);


// Private Data Members
private:

    /// Owning simulator
    Simulator& m_simulator;

    /// Number of simulation steps.
    StepNumber m_stepNumber = 0;

    /// Number of iterations.
    StepNumber m_iterations = 0;

    /// Simulation step.
    units::Duration m_timeStep;

    /// Real-time time step
    bool m_timeStepRealTime = true;

    /// World size.
    Vector<units::Length> m_worldSize{units::um(400), units::um(400)};

    /// Cache for loaded libraries.
    std::map<std::string, std::unique_ptr<Library>> m_libraries;

    /// Simulation modules.
    ModuleContainer m_modules;

#ifdef ENABLE_PHYSICS
    b2World m_world;
#endif

    /// Simulation objects.
    ObjectContainer m_objects;

    /// A map of preddefined programs.
    std::map<std::string, Program> m_programs;

#ifdef ENABLE_RENDER
    /// List of objects that requires init.
    std::vector<Object*> m_drawInitList;
#endif

#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    bool m_drawPhysics = true;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
