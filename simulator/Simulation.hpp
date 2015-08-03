/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/Log.hpp"
#include "core/String.hpp"
#include "core/Map.hpp"
#include "core/UniquePtr.hpp"
#include "core/ViewPtr.hpp"
#include "core/DynamicArray.hpp"
#include "core/TimeMeasurement.hpp"
#include "core/ListenerContainer.hpp"
#include "core/DataTable.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/Program.hpp"
#include "simulator/SimulationListener.hpp"

#if ENABLE_RENDER
#include "core/TriBool.hpp"
#include "render/Context.hpp"
#include "render/Color.hpp"
#endif

#if ENABLE_PHYSICS
// Box2D
#include <Box2D/Box2D.h>
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulator;
class Configuration;

/* ************************************************************************ */

/**
 * @brief Type for iteration number.
 */
using IterationNumber = unsigned long long;

/* ************************************************************************ */

/**
 * @brief Simulation class.
 */
class Simulation
    : public ListenerContainer<SimulationListener>
{


// Public Types
public:


    /// Type of simulation parameter value.
    using ParameterValueType = float;

    /// Module container type.
    using ModuleContainer = Map<String, UniquePtr<Module>>;

    /// Object container type.
    using ObjectContainer = DynamicArray<UniquePtr<Object>>;

    /// Object container type for object views.
    using ObjectViewContainer = DynamicArray<ViewPtr<Object>>;

    /// Container type for programs.
    using ProgramContainer = Map<String, Program>;

    /// Parameter container.
    using ParameterContainer = Map<String, ParameterValueType>;

    /// Initialization function.
    using Initializer = std::function<void(Simulation&)>;

    /// Initializer container type.
    using InitializerContainer = DynamicArray<Initializer>;

    /// Data table container type.
    using DataTableContainer = Map<String, DataTable>;


// Public Ctors
public:


    /**
     * @brief Constructor.
     */
    Simulation() noexcept;


    /**
     * @brief Virtual destructor.
     */
    virtual ~Simulation();


// Public Accessors
public:


    /**
     * @brief Returns if simulation is initialized.
     *
     * @return
     */
    bool isInitialized() const noexcept
    {
        return m_initialized;
    }


    /**
     * @brief Returns current iteration.
     *
     * @return
     */
    IterationNumber getIteration() const noexcept
    {
        return m_iteration;
    }


    /**
     * @brief Returns a number of total iterations.
     *
     * @return
     */
    IterationNumber getIterations() const noexcept
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
    units::Time getTimeStep() const noexcept
    {
        return m_timeStep;
    }


    /**
     * @brief Returns total simulation time.
     *
     * @return
     */
    units::Time getTotalTime() const noexcept
    {
        return m_totalTime;
    }


    /**
     * @brief If real-time step is enabled.
     *
     * @return
     */
    bool isTimeStepRealTime() const noexcept
    {
        return m_timeStep == Zero;
    }


    /**
     * @brief Returns if plugin with given name is loaded.
     *
     * @param name Plugin name.
     *
     * @return If plugin is loaded.
     */
    bool isPluginLoaded(const String& name) const noexcept
    {
        return m_plugins.find(name) != m_plugins.end();
    }


    /**
     * @brief If simulation uses given module.
     *
     * @param name Module name.
     *
     * @return
     */
    bool hasModule(const String& name) const noexcept
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
    Module* getModule(const String& name) noexcept
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
    ModuleType* getModule(const String& name) noexcept
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
    const SizeVector& getWorldSize() const noexcept
    {
        return m_worldSize;
    }


    /**
     * @brief Returns simulation parameters.
     *
     * @return
     */
    const ParameterContainer& getParameters() const noexcept
    {
        return m_parameters;
    }


    /**
     * @brief Check if simulation parameter exists.
     *
     * @param name Parameter name.
     *
     * @return
     */
    bool hasParameters(const String& name) const noexcept
    {
        return m_parameters.find(name) != m_parameters.end();
    }


    /**
     * @brief Returns parameter by name.
     *
     * @param name Parameter name.
     * @param def  Default value if parameter is not set.
     *
     * @return Parameter value or default value.
     */
    ParameterValueType getParameter(const String& name, ParameterValueType def = {}) const noexcept
    {
        auto it = m_parameters.find(name);
        return it != m_parameters.end() ? it->second : def;
    }


#if ENABLE_PHYSICS
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


#if ENABLE_PHYSICS
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
    const ProgramContainer& getPrograms() const noexcept
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
    bool hasProgram(const String& name) const noexcept
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
    Program getProgram(const String& name) noexcept
    {
        auto it = m_programs.find(name);

        // Try to build program from module
        if (it == m_programs.end())
            return buildProgram(name);

        return it->second;
    }


    /**
     * @brief Returns data table exists.
     *
     * @param name Data table name.
     *
     * @return
     */
    bool hasDataTable(const StringView& name) const noexcept
    {
        return m_dataTables.find(name.getData()) != m_dataTables.end();
    }


    /**
     * @brief Returns data table with given name.
     *
     * @param name Data table name.
     *
     * @return Data table.
     */
    DataTable& getDataTable(const StringView& name) noexcept
    {
        return m_dataTables[name.getData()];
    }


    /**
     * @brief Returns data table with given name.
     *
     * @param name Data table name.
     *
     * @return Data table.
     */
    const DataTable& getDataTable(const StringView& name) const
    {
        auto it = m_dataTables.find(name.getData());

        if (it == m_dataTables.end())
            throw InvalidArgumentException("Unable to find data table: " + String(name));

        return it->second;
    }


#if ENABLE_RENDER
    /**
     * @brief Get vizualize flag.
     *
     * @return
     */
    TriBool getVizualize() const noexcept
    {
        return m_vizualize;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Get background color.
     *
     * @return
     */
    render::Color getBackgroundColor() const noexcept
    {
        return m_backgroundColor;
    }
#endif


// Public Mutators
public:


    /**
     * @brief Register initializer.
     *
     * @param init Initializer function.
     */
    void addInitializer(Initializer init)
    {
        m_initializers.push_back(std::move(init));
    }


    /**
     * @brief Set total number of simulation iterations.
     *
     * @param iterations Number of iterations. If value is 0, there is
     *                   unlimited number of iterations.
     */
    void setIterations(IterationNumber iterations) noexcept
    {
        m_iterations = iterations;
    }


    /**
     * @brief Set simulation time step.
     *
     * @param dt Time step. If value is units::Zero, real-time step is enabled.
     */
    void setTimeStep(units::Time dt) noexcept
    {
        m_timeStep = dt;
    }


    /**
     * @brief Sets simulation parameters.
     *
     * @param parameters
     */
    void setParameters(ParameterContainer parameters) noexcept
    {
        m_parameters = std::move(parameters);
    }


    /**
     * @brief Sets simulation parameter by name.
     *
     * @param name  Parameter name.
     * @param value Parameter value.
     */
    void setParameter(const String& name, ParameterValueType value) noexcept
    {
        m_parameters[name] = value;
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
        return addModule(std::move(name), makeUnique<T>(std::forward<Args>(args)...));
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
    void setWorldSize(SizeVector size) noexcept
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
        return addObject(makeUnique<T>(*this, std::forward<Args>(args)...));
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


    /**
     * @brief Add request to object deletion.
     *
     * @param obj Pointer to object.
     *
     * @note Object is not directly deleted, it just queued to be deleted (in update()).
     * This is important in case the object is deleted from within (some program want
     * to delete object) and direct deletion will cause crash.
     */
    void deleteObject(ViewPtr<Object> obj)
    {
        m_objectsToDelete.push_back(std::move(obj));
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


    /**
     * @brief Create new data table.
     *
     * @param name Data table name.
     *
     * @return Created data table.
     */
    DataTable& addDataTable(const StringView& name)
    {
        return m_dataTables[name.getData()];
    }


#if ENABLE_RENDER
    /**
     * @brief Set vizualize flag.
     *
     * @param value
     */
    void setVizualize(TriBool value) noexcept
    {
        m_vizualize = value;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Set background color.
     *
     * @param color
     */
    void setBackgroundColor(render::Color color) noexcept
    {
        m_backgroundColor = color;
    }
#endif


// Public Operations
public:


    /**
     * @brief Reset simulation.
     */
    void reset();


    /**
     * @brief Initialize simulation.
     */
    void initialize();


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
     * @brief Loads required plugin with given name.
     *
     * @param name Plugin name.
     *
     * @return API to loaded plugin.
     *
     * @throw In case the plugin cannot be loaded.
     */
    ViewPtr<PluginApi> requirePlugin(const String& name);


    /**
     * @brief Loads plugin with given name.
     *
     * In case the library was loaded before, it't not loaded again.
     *
     * @param name Plugin name.
     *
     * @return Pointer to plugin API or nullptr if plugin cannot be loaded.
     *
     * @see requirePlugin
     */
    ViewPtr<PluginApi> loadPlugin(const String& name) noexcept;


    /**
     * @brief Store data tables into files.
     */
    void storeDataTables();


// Private Data Members
private:

    /// If simulation is initialized.
    bool m_initialized = false;

    /// A list of simulation initializers.
    InitializerContainer m_initializers;

    /// Number of simulation steps.
    IterationNumber m_iteration = 0;

    /// Number of iterations.
    IterationNumber m_iterations = 0;

    /// Simulation step.
    units::Time m_timeStep = Zero;

    /// Total simulation time.
    units::Time m_totalTime = Zero;

    /// World size.
    SizeVector m_worldSize{ units::um(400), units::um(400) };

    /// Parameters.
    ParameterContainer m_parameters;

    /// Used plugins.
    Map<String, ViewPtr<PluginApi>> m_plugins;

    /// Simulation modules.
    ModuleContainer m_modules;

#ifdef ENABLE_PHYSICS
    b2World m_world;
#endif

    /// Simulation objects.
    ObjectContainer m_objects;

    /// Objects that should be deleted.
    ObjectViewContainer m_objectsToDelete;

    /// A map of preddefined programs.
    ProgramContainer m_programs;

    /// Managed data tables.
    DataTableContainer m_dataTables;

#if ENABLE_RENDER
    /// Option if vizualization should be enabled.
    TriBool m_vizualize = Indeterminate;
#endif

#if ENABLE_RENDER
    /// Background (clear) color.
    render::Color m_backgroundColor = render::colors::WHITE;
#endif

#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    bool m_drawPhysics = true;
#endif
};

/* ************************************************************************ */

/**
 * @brief Time measurement functor with printing current iteration.
 */
struct TimeMeasurementIterationOutput
{
    /// Simulation.
    ViewPtr<const Simulation> m_simulation;


    /**
     * @brief Constructor.
     *
     * @param sim Simulation.
     */
    explicit TimeMeasurementIterationOutput(ViewPtr<const Simulation> sim)
        : m_simulation(sim)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param sim Simulation
     */
    explicit TimeMeasurementIterationOutput(const Simulation& sim)
        : m_simulation(&sim)
    {
        // Nothing to do
    }


    /**
     * @brief Functor function.
     *
     * @param out  Output stream.
     * @param name Measurement name.
     * @param dt   Measured time.
     */
    void operator()(OutStream& out, const String& name, Clock::duration dt) const noexcept
    {
        using namespace std::chrono;
        out << name << ";" << m_simulation->getIteration() << ";" << duration_cast<microseconds>(dt).count() << "\n";
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
