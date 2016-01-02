/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

// C++
#include <cassert>
#include <functional>

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/UnitsCtors.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/DataTable.hpp"
#include "cece/core/Parameters.hpp"
#include "cece/core/OutStream.hpp"
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
#include "cece/plugin/Library.hpp"

#ifdef CECE_ENABLE_RENDER
#include "cece/render/Context.hpp"
#include "cece/render/Color.hpp"
#include "cece/render/Object.hpp"
#if CONFIG_RENDER_TEXT_ENABLE
#include "cece/render/Font.hpp"
#endif
#endif

// Box2D
#ifdef CECE_ENABLE_BOX2D_PHYSICS
#  include <Box2D/Box2D.h>
#  include "ConverterBox2D.hpp"
#endif

/* ************************************************************************ */

namespace cece { namespace plugin { class Context; } }
namespace cece { namespace config { class Configuration; } }

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Type for iteration number.
 */
using IterationNumber = unsigned long long;

/* ************************************************************************ */

/**
 * @brief Type for iteration count.
 */
using IterationCount = IterationNumber;

/* ************************************************************************ */

/**
 * @brief Simulation class.
 */
class Simulation
{


// Public Types
public:


    /// Type of simulation parameter value.
    using ParameterValueType = float;

    /// Data table container type.
    using DataTableContainer = Map<String, DataTable>;


// Public Ctors
public:


    /**
     * @brief Constructor.
     *
     * @param context Plugin context.
     */
    explicit Simulation(plugin::Context& context) noexcept;


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

        assert(dynamic_cast<ModuleType*>(module.get()));
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
        return m_world;
    }


    /**
     * @brief Returns physics world.
     *
     * @return
     */
    const b2World& getWorld() const noexcept
    {
        return m_world;
    }


    /**
     * @brief Returns physics engine time step.
     *
     * @return
     */
    units::Time getPhysicsEngineTimeStep() const noexcept
    {
        return m_converter.getTimeStepBox2D();
    }


    /**
     * @brief Returns Box2D units converter.
     *
     * @return
     */
    ConverterBox2D& getConverter() noexcept
    {
        return m_converter;
    }


    /**
     * @brief Returns Box2D units converter.
     *
     * @return
     */
    const ConverterBox2D& getConverter() const noexcept
    {
        return m_converter;
    }

#endif


    /**
     * @brief Returns maximum translation vector magnitude per iteration.
     *
     * @return
     */
    units::Length getMaxObjectTranslation() const noexcept
    {
#ifdef CECE_ENABLE_BOX2D_PHYSICS
        return m_converter.getMaxObjectTranslation();
#else
        return units::Length{1e3};
#endif
    }


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


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Returns font color.
     *
     * return
     */
    const render::Color& getFontColor() const noexcept
    {
        return m_fontColor;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Get font size.
     *
     * @return
     */
    unsigned int getFontSize() const noexcept
    {
        return m_fontSize;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Returns if simulation time should be rendered.
     *
     * @return
     */
    bool isSimulationTimeRender() const noexcept
    {
        return m_simulationTimeRender;
    }
#endif


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
    void setTimeStep(units::Time dt)
    {
        if (dt == Zero)
            throw InvalidArgumentException("Time step cannot be zero");

        m_timeStep = dt;

#ifdef CECE_ENABLE_BOX2D_PHYSICS
        m_converter.setTimeStep(dt);
#endif
    }


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
    void setPhysicsEngineTimeStep(units::Time dt) noexcept
    {
        m_converter.setTimeStepBox2D(dt);
    }

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


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Set font color.
     *
     * @param color
     */
    void setFontColor(render::Color color) noexcept
    {
        m_fontColor = color;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Set font size.
     *
     * @param size
     */
    void setFontSize(unsigned int size) noexcept
    {
        m_fontSize = size;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Set if simulation time should be rendered.
     *
     * @param flag
     */
    void setSimulationTimeRender(bool flag) noexcept
    {
        m_simulationTimeRender = flag;
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
     * @brief Configure simulation.
     *
     * @param config
     */
    void configure(const config::Configuration& config);


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
    bool update()
    {
        return update(getTimeStep());
    }


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
     *
     * @param dt Time step.
     */
    void updateModules(units::Time dt);


    /**
     * @brief Update objects.
     *
     * @param dt Time step.
     */
    void updateObjects(units::Time dt);


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
    b2World m_world;

    // Box2D units converter.
    ConverterBox2D m_converter;
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

#if CONFIG_RENDER_TEXT_ENABLE
    /// Font renderer.
    render::ObjectPtr<render::Font> m_font;
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    /// Font color.
    render::Color m_fontColor = render::colors::WHITE;
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    /// Font size.
    unsigned int m_fontSize = 30;
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    /// If time should be rendered.
    bool m_simulationTimeRender = false;
#endif

    /// Outstream for simulation objects data.
    UniquePtr<OutStream> m_dataOutObjects;

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
}

/* ************************************************************************ */
