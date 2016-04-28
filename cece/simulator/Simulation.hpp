/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/export.hpp"
#include "cece/core/Assert.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/UnitsCtors.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/DataTable.hpp"
#include "cece/core/Parameters.hpp"
#include "cece/core/InOutStream.hpp"
#include "cece/init/Initializer.hpp"
#include "cece/init/Container.hpp"
#include "cece/module/Module.hpp"
#include "cece/module/Container.hpp"
#include "cece/object/Object.hpp"
#include "cece/object/Container.hpp"
#include "cece/object/Type.hpp"
#include "cece/object/TypeContainer.hpp"
#include "cece/program/Program.hpp"
#include "cece/program/NamedContainer.hpp"
#include "cece/simulator/IterationType.hpp"

#ifdef CECE_ENABLE_RENDER
#include "cece/render/Color.hpp"
#endif

/* ************************************************************************ */

class b2World;
namespace cece { namespace plugin { class Context; } }
namespace cece { namespace plugin { class Api; } }
namespace cece { namespace config { class Configuration; } }

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Simulation class.
 */
class CECE_EXPORT Simulation
{


// Public Types
public:


    /// Type of simulation parameter value.
    using ParameterValueType = Parameters::ValueType;

    /// Data table container type.
    using DataTableContainer = Map<String, DataTable>;


// Public Ctors
public:


    /**
     * @brief Constructor.
     *
     * @param context Plugin context.
     * @param path    Path to simulation file.
     */
    explicit Simulation(plugin::Context& context, FilePath path = "<memory>") noexcept;


    /**
     * @brief Virtual destructor.
     */
    virtual ~Simulation();


// Public Accessors
public:


    /**
     * @brief Returns plugin context.
     *
     * @return
     */
    plugin::Context& getPluginContext() noexcept
    {
        return m_pluginContext;
    }


    /**
     * @brief Returns plugin context.
     *
     * @return
     */
    const plugin::Context& getPluginContext() const noexcept
    {
        return m_pluginContext;
    }


    /**
     * @brief Returns simulation file name.
     *
     * @return
     */
    const FilePath& getFileName() const noexcept
    {
        return m_fileName;
    }


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
    const Map<String, ViewPtr<plugin::Api>>& getPlugins() const noexcept
    {
        return m_plugins;
    }


    /**
     * @brief Return a list of modules.
     *
     * @return
     */
    const module::Container& getModules() const noexcept
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
     * @brief Returns gravity vector.
     *
     * @return
     */
    AccelerationVector getGravity() const noexcept;


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
    bool hasModule(StringView name) const noexcept
    {
        return m_modules.exists(name);
    }


    /**
     * @brief Get module by name.
     *
     * @param name Module name.
     *
     * @return Pointer to module. If module doesn't exists, nullptr is returned.
     */
    ViewPtr<module::Module> getModule(StringView name) noexcept
    {
        return m_modules.get(name);
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

        Assert(dynamic_cast<ModuleType*>(module.get()));
        return static_cast<ModuleType*>(module.get());
    }


    /**
     * @brief Return a list of world objects.
     *
     * @return
     */
    object::Container& getObjects() noexcept
    {
        return m_objects;
    }


    /**
     * @brief Return a list of world objects.
     *
     * @return
     */
    const object::Container& getObjects() const noexcept
    {
        return m_objects;
    }


    /**
     * @brief Return a number of objects.
     *
     * @return
     */
    unsigned long getObjectCount() const noexcept
    {
        return m_objects.getCount();
    }


    /**
     * @brief Return a number of objects by type.
     *
     * @param className Name of required class.
     *
     * @return
     */
    unsigned long getObjectCountType(StringView className) const noexcept
    {
        return m_objects.getCountByType(className);
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
    Parameters& getParameters() noexcept
    {
        return m_parameters;
    }


    /**
     * @brief Returns simulation parameters.
     *
     * @return
     */
    const Parameters& getParameters() const noexcept
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
        return m_parameters.exists(name);
    }


    /**
     * @brief Returns parameter by name.
     *
     * @param name Parameter name.
     *
     * @return Parameter value.
     */
    ParameterValueType getParameter(const String& name) const
    {
        return m_parameters.get(name);
    }


    /**
     * @brief Returns parameter by name.
     *
     * @param name Parameter name.
     * @param def  Default value if parameter is not set.
     *
     * @return Parameter value or default value.
     */
    ParameterValueType getParameter(const String& name, ParameterValueType def) const noexcept
    {
        return m_parameters.get(name, def);
    }


    /**
     * @brief Find object class by name.
     *
     * @param name
     *
     * @return
     */
    ViewPtr<const object::Type> findObjectType(StringView name) const noexcept;


#ifdef CECE_ENABLE_BOX2D_PHYSICS

    /**
     * @brief Returns physics world.
     *
     * @return
     */
    b2World& getWorld() noexcept
    {
        return *m_world;
    }


    /**
     * @brief Returns physics world.
     *
     * @return
     */
    const b2World& getWorld() const noexcept
    {
        return *m_world;
    }


    /**
     * @brief Returns physics engine time step.
     *
     * @return
     */
    units::Time getPhysicsEngineTimeStep() const noexcept;

#endif


    /**
     * @brief Returns maximum translation vector magnitude per iteration.
     *
     * @return
     */
    units::Length getMaxObjectTranslation() const noexcept;


#if defined(CECE_ENABLE_RENDER) && defined(CECE_ENABLE_BOX2D_PHYSICS) && defined(CECE_ENABLE_BOX2D_PHYSICS_DEBUG)

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
    const program::NamedContainer& getPrograms() const noexcept
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
        return m_programs.exists(name);
    }


    /**
     * @brief Returns program.
     *
     * @param name Program name.
     *
     * @return Pointer to program.
     */
    UniquePtr<program::Program> getProgram(StringView name)
    {
        auto ptr = m_programs.get(name);

        // Try to build program from module
        if (ptr == nullptr)
            return buildProgram(name);

        // Clone stored program
        return ptr->clone();
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


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Get visualize flag.
     *
     * @return
     */
    bool isVisualized() const noexcept
    {
        return m_visualized;
    }


    /**
     * @brief Get background color.
     *
     * @return
     */
    const render::Color& getBackgroundColor() const noexcept
    {
        return m_backgroundColor;
    }

#endif


    /**
     * @brief Returns simulation resource.
     *
     * @param name Resource name.
     *
     * @return Pointer to resource stream or nullptr.
     */
    UniquePtr<InOutStream> getResource(const String& name) noexcept;


// Public Mutators
public:


    /**
     * @brief Register initializer.
     *
     * @param initializer Initializer program.
     */
    void addInitializer(UniquePtr<init::Initializer> initializer)
    {
        m_initializers.add(std::move(initializer));
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
     * @param dt Time step.
     */
    void setTimeStep(units::Time dt);


    /**
     * @brief Sets simulation parameters.
     *
     * @param parameters
     */
    void setParameters(Parameters parameters) noexcept
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
        m_parameters.set(name, value);
    }


    /**
     * @brief Change simulation world gravity.
     *
     * @param gravity
     */
    void setGravity(const AccelerationVector& gravity) noexcept;


    /**
     * @brief Register object class.
     *
     * @param rec.
     */
    void addObjectType(object::Type rec)
    {
        m_objectClasses.add(std::move(rec));
    }


    /**
     * @brief Add new module.
     *
     * @param name   Module name.
     * @param module Pointer to module.
     *
     * @return A pointer to inserted module.
     */
    ViewPtr<module::Module> addModule(String name, UniquePtr<module::Module> module)
    {
        return m_modules.add(std::move(name), std::move(module));
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
    ViewPtr<T> addModule(String name, UniquePtr<T> mod)
    {
        return addModule(std::move(name), UniquePtr<module::Module>(mod));
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
    ViewPtr<T> createModule(String name, Args&&... args)
    {
        return addModule(std::move(name), makeUnique<T>(std::forward<Args>(args)...));
    }


    /**
     * @brief Load module from library and add it into simulation.
     *
     * @param path      Path to module.
     * @param storePath Path under what is module stored and can be accessed.
     *                  If is empty, `path` is used. This is useful when module
     *                  extends functionality of another module but the behaviour
     *                  is same as original module.
     *
     * @return A pointer to created module.
     */
    ViewPtr<module::Module> useModule(const String& path, String storePath = {});


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
    ViewPtr<ModuleType> useModule(const String& path)
    {
        return useModule(path);
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
    ViewPtr<T> addObject(UniquePtr<T> obj)
    {
        Assert(obj);
        return m_objects.addObject(std::move(obj));
    }


    /**
     * @brief Create a new object.
     *
     * @param args...
     *
     * @return
     */
    template<typename T, typename... Args>
    ViewPtr<T> createObject(Args&&... args)
    {
        return addObject(makeUnique<T>(*this, std::forward<Args>(args)...));
    }


    /**
     * @brief Build object by name.
     *
     * @param name Object name.
     * @param type Type of created object.
     *
     * @return Created object.
     */
    object::Object* buildObject(const String& name, object::Object::Type type = object::Object::Type::Dynamic);


    /**
     * @brief Add request to object deletion.
     *
     * @param obj Pointer to object.
     *
     * @note Object is not directly deleted, it just queued to be deleted (in update()).
     * This is important in case the object is deleted from within (some program want
     * to delete object) and direct deletion will cause crash.
     */
    void deleteObject(ViewPtr<object::Object> obj)
    {
        m_objects.deleteObject(obj);
    }


#ifdef CECE_ENABLE_BOX2D_PHYSICS

    /**
     * @brief Set physics engine simulation time step.
     *
     * @param dt Time step.
     */
    void setPhysicsEngineTimeStep(units::Time dt) noexcept;

#endif


#if defined(CECE_ENABLE_RENDER) && defined(CECE_ENABLE_BOX2D_PHYSICS) && defined(CECE_ENABLE_BOX2D_PHYSICS_DEBUG)

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
    void addProgram(String name, UniquePtr<program::Program> program)
    {
        m_programs.add(std::move(name), std::move(program));
    }


    /**
     * @brief Create a program from module.
     *
     * @param name Program name.
     *
     * @return Created program.
     */
    UniquePtr<program::Program> buildProgram(StringView name);


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


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Set visualize flag.
     *
     * @param value
     */
    void setVisualized(bool value) noexcept
    {
        m_visualized = value;
    }


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
     *
     * @param termFlag Termination flag.
     */
    void initialize(AtomicBool& termFlag);


    /**
     * @brief Configure simulation.
     *
     * @param config
     */
    void loadConfig(const config::Configuration& config);


    /**
     * @brief Store simulation configuration.
     *
     * @param config
     */
    void storeConfig(config::Configuration& config) const;


    /**
     * @brief Update simulation.
     *
     * @return If next step can be calculated.
     */
    bool update();


#ifdef CECE_ENABLE_RENDER

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
    ViewPtr<plugin::Api> requirePlugin(const String& name);


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
    ViewPtr<plugin::Api> loadPlugin(const String& name) noexcept;


    /**
     * @brief Store data tables into files.
     */
    void storeDataTables();


    /**
     * @brief Calculate coefficient used to transform simulation units (time step)
     * to physical engine.
     *
     * @return
     */
    RealType calcPhysicalEngineCoefficient() const noexcept
    {
        return getPhysicsEngineTimeStep() / getTimeStep();
    }


    /**
     * @brief Find object at qiven position.
     *
     * @param position
     *
     * @return
     */
    ViewPtr<object::Object> query(const PositionVector& position) const noexcept;


// Protected Operations
protected:


    /**
     * @brief Update modules.
     */
    void updateModules();


    /**
     * @brief Update objects.
     */
    void updateObjects();


    /**
     * @brief Detect objects that cannot be visible in the scene and can be
     * safely deleted.
     */
    void detectDeserters();


    /**
     * @brief Delete objects marked for deletion.
     */
    void deleteObjects();


// Private Data Members
private:

    /// Simulation plugin context.
    plugin::Context& m_pluginContext;

    /// Path of simulation file.
    FilePath m_fileName;

    /// If simulation is initialized.
    bool m_initialized = false;

    /// A list of simulation initializers.
    init::Container m_initializers;

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

    /// Simulation parameters.
    Parameters m_parameters;

    /// Used plugins.
    Map<String, ViewPtr<plugin::Api>> m_plugins;

    /// Simulation modules.
    module::Container m_modules;

#ifdef CECE_ENABLE_BOX2D_PHYSICS
    /// Box2D world
    UniquePtr<b2World> m_world;
#endif

    /// Simulation objects.
    object::Container m_objects;

    /// A map of preddefined programs.
    program::NamedContainer m_programs;

    /// Managed data tables.
    DataTableContainer m_dataTables;

    /// Registered object classes.
    object::TypeContainer m_objectClasses;

#ifdef CECE_ENABLE_RENDER

    /// Option if visualization is enabled.
    bool m_visualized = true;

    /// Background (clear) color.
    render::Color m_backgroundColor = render::colors::WHITE;

#endif

#if defined(CECE_ENABLE_RENDER) && defined(CECE_ENABLE_BOX2D_PHYSICS) && defined(CECE_ENABLE_BOX2D_PHYSICS_DEBUG)
    bool m_drawPhysics = false;
#endif

    /// Outstream for simulation objects data.
    UniquePtr<OutStream> m_dataOutObjects;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
