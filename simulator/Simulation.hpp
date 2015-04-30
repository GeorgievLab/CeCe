
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


    /// Object builder type.
    using ObjectBuilder = std::function<std::unique_ptr<Object>(Simulation&, const ConfigurationBase&)>;

    /// Module container type.
    using ModuleContainer = std::map<std::string, std::unique_ptr<Module>>;

    /// Object container type.
    using ObjectContainer = std::vector<std::unique_ptr<Object>>;

    /// Object builders container type.
    using ObjectBuilderContainer = std::map<std::string, ObjectBuilder>;


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


// Public Mutators
public:


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
     * @brief Register object builder.
     *
     * @param name    Object name.
     * @param builder Builder.
     */
    void registerObjectBuilder(std::string name, ObjectBuilder builder)
    {
        if (m_objectBuilders.find(name) != m_objectBuilders.end())
            Log::warning("Replacing builder: ", name);

        m_objectBuilders.emplace(std::move(name), std::move(builder));
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
     * @param name   Object name.
     * @param config Object configuration.
     *
     * @return
     */
    Object* buildObject(const std::string& name, const ConfigurationBase& config)
    {
        auto it = m_objectBuilders.find(name);
        if (it == m_objectBuilders.end())
            throw std::runtime_error("Unable to create object: " + name);

        return addObject(it->second(*this, config));
    }


#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    /**
     * @brief If physics debug data should be shown.
     */
    void setDrawPhysics(bool flag) noexcept
    {
        m_drawPhysics = flag;
    }
#endif


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
     */
    void update(units::Duration dt) noexcept;


    /**
     * @brief Update simulation by time step.
     */
    void update()
    {
        update(getTimeStep());
    }


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


// Private Data Members
private:

    /// Owning simulator
    Simulator& m_simulator;

    /// Number of simulation steps.
    StepNumber m_stepNumber = 0;

    /// Simulation step.
    units::Duration m_timeStep;

    /// Simulation modules.
    ModuleContainer m_modules;

    /// World size.
    Vector<units::Length> m_worldSize{units::um(400), units::um(400)};

    /// Simulation objects.
    ObjectContainer m_objects;

    /// Registered object builders.
    ObjectBuilderContainer m_objectBuilders;

#ifdef ENABLE_RENDER
    /// List of objects that requires init.
    std::vector<Object*> m_drawInitList;
#endif

#ifdef ENABLE_PHYSICS
    b2World m_world;
#endif

#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    bool m_drawPhysics = true;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
