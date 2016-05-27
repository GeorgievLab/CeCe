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

// C++
#include <cstddef>

// CeCe
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
#include "cece/render/Context.hpp"
#include "cece/simulator/Visualization.hpp"
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
class Simulation
{

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


////////


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
     * @brief Returns world size.
     *
     * @return
     */
    const SizeVector& getWorldSize() const noexcept
    {
        return m_worldSize;
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
     * @brief Returns if there are unlimited iterations.
     *
     * @return
     */
    bool hasUnlimitedIterations() const noexcept
    {
        return getIterations() == 0;
    }


    /**
     * @brief Returns current simulation time step.
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


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Returns visualization
     *
     * @return
     */
    Visualization& getVisualization() noexcept
    {
        return m_visualization;
    }


    /**
     * @brief Returns visualization
     *
     * @return
     */
    const Visualization& getVisualization() const noexcept
    {
        return m_visualization;
    }

#endif


////////

#if OFF
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
     * @brief Returns simulation modules.
     *
     * @return
     */
    module::Container& getModules() noexcept
    {
        return m_modules;
    }


    /**
     * @brief Returns simulation modules.
     *
     * @return
     */
    const module::Container& getModules() const noexcept
    {
        return m_modules;
    }


    /**
     * @brief Returns simulation objects.
     *
     * @return
     */
    object::Container& getObjects() noexcept
    {
        return m_objects;
    }


    /**
     * @brief Returns simulation objects.
     *
     * @return
     */
    const object::Container& getObjects() const noexcept
    {
        return m_objects;
    }


    /**
     * @brief Returns global programs.
     *
     * @return
     */
    program::NamedContainer& getPrograms() noexcept
    {
        return m_programs;
    }


    /**
     * @brief Returns global programs.
     *
     * @return
     */
    const program::NamedContainer& getPrograms() const noexcept
    {
        return m_programs;
    }
#endif

/////////


    /**
     * @brief Returns simulation resource.
     *
     * @param name Resource name.
     *
     * @return Pointer to resource stream or nullptr.
     */
    UniquePtr<InOutStream> getResource(StringView name) noexcept;


    /**
     * @brief Returns if simulation parameter exists.
     *
     * @param name Parameter name.
     *
     * @return
     */
    bool hasParameter(StringView name) const noexcept;


    /**
     * @brief Returns simulation parameter.
     *
     * @param name Parameter name.
     *
     * @return Parameter value.
     *
     * @throw RuntimeException
     */
    const String& getParameter(StringView name) const;


    /**
     * @brief Returns if module with given name exists.
     *
     * @param name Module name.
     *
     * @return
     */
    bool hasModule(StringView name) const noexcept;


    /**
     * @brief Returns required module.
     *
     * @param name Module name.
     *
     * @return
     */
    ViewPtr<module::Module> getModule(StringView name) const noexcept;


    /**
     * @brief Returns required module.
     *
     * @tparam ModuleType
     *
     * @param name Module name.
     *
     * @return
     */
    template<typename ModuleType>
    ViewPtr<ModuleType> getModule(StringView name) const noexcept
    {
        auto module = getModule(name);

        if (!module)
            return nullptr;

        Assert(dynamic_cast<ModuleType*>(module.get()));
        return static_cast<ModuleType*>(module.get());
    }


    /**
     * @brief Returns required module or throw.
     *
     * @param name Module name.
     *
     * @return
     *
     * @throw RuntimeException
     */
    ViewPtr<module::Module> requireModule(StringView name) const;


    /**
     * @brief Returns required module or throw.
     *
     * @tparam ModuleType
     *
     * @param name Module name.
     *
     * @return
     *
     * @throw RuntimeException
     */
    template<typename ModuleType>
    ViewPtr<ModuleType> requireModule(StringView name) const
    {
        auto module = requireModule(name);
        Assert(dynamic_cast<ModuleType*>(module.get()));
        return static_cast<ModuleType*>(module.get());
    }


/////////


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


// Public Mutators
public:


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
     * @brief Change world size.
     *
     * @param size
     */
    void setWorldSize(SizeVector size) noexcept
    {
        m_worldSize = std::move(size);
    }


    /**
     * @brief Set simulation time step.
     *
     * @param dt Time step.
     */
    void setTimeStep(units::Time dt);


///////////


    /**
     * @brief Set simulation parameter.
     *
     * @param name  Parameter name.
     * @param value Parameter value.
     */
    void setParameter(String name, String value);


    /**
     * @brief Register an initializer.
     *
     * @param initializer Initializer.
     *
     * @return
     */
    ViewPtr<init::Initializer> addInitializer(UniquePtr<init::Initializer> initializer);


    /**
     * @brief Register an initializer.
     *
     * @param name Initializer name.
     *
     * @return
     */
    ViewPtr<init::Initializer> createInitializer(StringView name);


    /**
     * @brief Register an initializer.
     *
     * @param name   Initializer name.
     * @param config Initializer configuration.
     *
     * @return
     */
    ViewPtr<init::Initializer> createInitializer(StringView name, const config::Configuration& config);


    /**
     * @brief Add a module.
     *
     * @param name   Module name.
     * @param module Added module.
     *
     * @return
     */
    ViewPtr<module::Module> addModule(String name, UniquePtr<module::Module> module);


    /**
     * @brief Add an object.
     *
     * @param object Added object.
     *
     * @return
     */
    ViewPtr<object::Object> addObject(UniquePtr<object::Object> object);


//////////


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
     * @brief Build object by name.
     *
     * @param name Object name.
     * @param type Type of created object.
     *
     * @return Created object.
     */
    object::Object* buildObject(const String& name, object::Object::Type type = object::Object::Type::Dynamic);



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


// Public Operations
public:


    /**
     * @brief Reset simulation.
     */
    void reset();


    /**
     * @brief Initialize simulation.
     *
     * @param flag Initialization flag.
     */
    void initialize(AtomicBool& flag);


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

    /// Registered object classes.
    object::TypeContainer m_objectClasses;

#ifdef CECE_ENABLE_RENDER
    /// Simulation visualization.
    Visualization m_visualization;
#endif

#if defined(CECE_ENABLE_RENDER) && defined(CECE_ENABLE_BOX2D_PHYSICS) && defined(CECE_ENABLE_BOX2D_PHYSICS_DEBUG)
    bool m_drawPhysics = false;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
