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
#include "cece/core/Units.hpp"
#include "cece/core/UnitsCtors.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/Parameters.hpp"
#include "cece/core/IterationType.hpp"
#include "cece/plugin/Context.hpp"
#include "cece/init/Container.hpp"
#include "cece/module/Container.hpp"
#include "cece/object/Container.hpp"
#include "cece/object/TypeContainer.hpp"
#include "cece/program/NamedContainer.hpp"
#include "cece/simulator/Simulation.hpp"

#ifdef CECE_ENABLE_RENDER
#include "cece/simulator/Visualization.hpp"
#endif

#ifdef CECE_THREAD_SAFE
#include "cece/core/Mutex.hpp"
#endif

/* ************************************************************************ */

namespace cece {
    namespace config { class Configuration; }
    namespace plugin { class Api; }
    namespace plugin { class Repository; }
}

#ifdef CECE_ENABLE_BOX2D_PHYSICS
class b2World;
#endif

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Default simulation class.
 */
class DefaultSimulation : public Simulation
{

// Public Ctors
public:


    /**
     * @brief Constructor.
     *
     * @param repository Plugin repository.
     * @param path       Path to simulation file.
     */
    explicit DefaultSimulation(const plugin::Repository& repository, FilePath path = "<memory>") noexcept;


    /**
     * @brief Destructor.
     */
    ~DefaultSimulation();


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
    module::Container& getModuleContainer() noexcept
    {
        return m_modules;
    }


    /**
     * @brief Returns simulation modules.
     *
     * @return
     */
    const module::Container& getModuleContainer() const noexcept
    {
        return m_modules;
    }


    /**
     * @brief Returns simulation objects.
     *
     * @return
     */
    object::Container& getObjectContainer() noexcept
    {
        return m_objects;
    }


    /**
     * @brief Returns simulation objects.
     *
     * @return
     */
    const object::Container& getObjectContainer() const noexcept
    {
        return m_objects;
    }


    /**
     * @brief Returns global programs.
     *
     * @return
     */
    program::NamedContainer& getProgramContainer() noexcept
    {
        return m_programs;
    }


    /**
     * @brief Returns global programs.
     *
     * @return
     */
    const program::NamedContainer& getProgramContainer() const noexcept
    {
        return m_programs;
    }


    /**
     * @brief Returns simulation file name.
     *
     * @return
     */
    const FilePath& getFileName() const noexcept override
    {
        return m_fileName;
    }


    /**
     * @brief Returns world size.
     *
     * @return
     */
    const units::SizeVector& getWorldSize() const noexcept override
    {
        return m_worldSize;
    }


    /**
     * @brief Returns if simulation is initialized.
     *
     * @return
     */
    bool isInitialized() const noexcept override
    {
        return m_initialized;
    }


    /**
     * @brief Returns current iteration.
     *
     * @return
     */
    IterationType getIteration() const noexcept override
    {
        return m_iteration;
    }


    /**
     * @brief Returns a number of total iterations.
     *
     * @return
     */
    IterationType getIterations() const noexcept override
    {
        return m_iterations;
    }


    /**
     * @brief Returns current simulation time step.
     *
     * @return
     */
    units::Time getTimeStep() const noexcept override
    {
        return m_timeStep;
    }


    /**
     * @brief Returns total simulation time.
     *
     * @return
     */
    units::Time getTotalTime() const noexcept override
    {
        return m_totalTime;
    }


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Returns visualization
     *
     * @return
     */
    Visualization& getVisualization() noexcept override
    {
        return m_visualization;
    }


    /**
     * @brief Returns visualization
     *
     * @return
     */
    const Visualization& getVisualization() const noexcept override
    {
        return m_visualization;
    }

#endif


    /**
     * @brief Returns simulation resource.
     *
     * @param name Resource name.
     *
     * @return Pointer to resource stream or nullptr.
     */
    UniquePtr<InOutStream> getResource(StringView name) noexcept override;


    /**
     * @brief Returns if simulation parameter exists.
     *
     * @param name Parameter name.
     *
     * @return
     */
    bool hasParameter(StringView name) const noexcept override;


    /**
     * @brief Returns simulation parameter.
     *
     * @param name Parameter name.
     *
     * @return Parameter value.
     *
     * @throw RuntimeException
     */
    String getParameter(StringView name) const override;


    /**
     * @brief Returns if module with given name exists.
     *
     * @param name Module name.
     *
     * @return
     */
    bool hasModule(StringView name) const noexcept override;


    /**
     * @brief Returns required module.
     *
     * @param name Module name.
     *
     * @return
     */
    ViewPtr<module::Module> getModule(StringView name) const noexcept override;


    /**
     * @brief Check if program with given name exists.
     *
     * @param name Program name.
     *
     * @return
     */
    bool hasProgram(StringView name) const noexcept override;


    /**
     * @brief Returns program with given name. At first global storage of
     * program checked and then program factory. In case of global storage
     * a copy is created so each object work with own data.
     *
     * @param name Program name.
     *
     * @return Pointer to program.
     */
    UniquePtr<program::Program> getProgram(StringView name) const override;


    /**
     * @brief Returns number of simulation objects.
     *
     * @return
     */
    std::size_t getObjectCount() const noexcept override;


    /**
     * @brief Returns number of simulation objects with given type.
     *
     * @param type Object type.
     *
     * @return
     */
    std::size_t getObjectCount(StringView type) const noexcept override;


    /**
     * @brief Returns all objects.
     *
     * @return
     */
    DynamicArray<ViewPtr<object::Object>> getObjects() const noexcept override;


    /**
     * @brief Returns all objects with given type.
     *
     * @param type Object type.
     *
     * @return
     */
    DynamicArray<ViewPtr<object::Object>> getObjects(StringView type) const noexcept override;


    /**
     * @brief Returns gravity vector.
     *
     * @return
     */
    units::AccelerationVector getGravity() const noexcept;


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


// Public Mutators
public:


    /**
     * @brief Load plugin into simulation. Loaded plugins allow to use their
     * symbols within simulation.
     *
     * @param name Plugin name.
     *
     * @return Plugin API.
     */
    ViewPtr<const plugin::Api> loadPlugin(StringView name) override;


    /**
     * @brief Unload plugin from simulation.
     *
     * @param name Plugin name.
     */
    void unloadPlugin(StringView name) override;


    /**
     * @brief Set total number of simulation iterations.
     *
     * @param iterations Number of iterations.
     */
    void setIterations(IterationType iterations) noexcept override
    {
        m_iterations = iterations;
    }


    /**
     * @brief Change world size.
     *
     * @param size Simulation world size.
     */
    void setWorldSize(units::SizeVector size) noexcept override
    {
        m_worldSize = std::move(size);
    }


    /**
     * @brief Set simulation time step.
     *
     * @param dt Time step.
     */
    void setTimeStep(units::Time dt) override;


    /**
     * @brief Set simulation parameter.
     *
     * @param name  Parameter name.
     * @param value Parameter value.
     */
    void setParameter(String name, String value) override;


    /**
     * @brief Add an initializer.
     *
     * @param initializer Initializer.
     *
     * @return
     */
    ViewPtr<init::Initializer> addInitializer(UniquePtr<init::Initializer> initializer) override;


    /**
     * @brief Create and register initializer.
     *
     * @param type Initializer type.
     *
     * @return
     */
    ViewPtr<init::Initializer> createInitializer(StringView type) override;


    /**
     * @brief Remove and delete initializer.
     *
     * @param initializer Initializer to delete.
     *
     * @return
     */
    void deleteInitializer(ViewPtr<init::Initializer> initializer) override;


    /**
     * @brief Add a module.
     *
     * @param name   Module name.
     * @param module Added module.
     *
     * @return
     */
    ViewPtr<module::Module> addModule(String name, UniquePtr<module::Module> module) override;


    /**
     * @brief Create and register a module.
     *
     * @param type Module type and registration name.
     *
     * @return
     */
    ViewPtr<module::Module> createModule(StringView type) override;


    /**
     * @brief Remove and delete module.
     *
     * @param name Module name.
     */
    void deleteModule(StringView name) override;


    /**
     * @brief Register object type.
     *
     * @param name   Object type name.
     * @param parent Parent type name.
     * @param config Type configuration.
     *
     * @return
     */
    void addObjectType(String name, String parent, const config::Configuration& config) override;


    /**
     * @brief Add an object.
     *
     * @param object Added object.
     *
     * @return
     */
    ViewPtr<object::Object> addObject(UniquePtr<object::Object> object) override;


    /**
     * @brief Create and register an object.
     *
     * @param type Object type.
     *
     * @return
     */
    ViewPtr<object::Object> createObject(StringView type) override;


    /**
     * @brief Create and register object.
     *
     * @param type Object type name.
     *
     * @return Pointer to created object.
     * @deprecated
     */
    ViewPtr<object::Object> createObject(StringView type, object::Object::Type state) override;


    /**
     * @brief Remove given object from simulation and delete it.
     *
     * @param object Pointer to deleted object.
     */
    void deleteObject(ViewPtr<object::Object> object) override;


    /**
     * @brief Register a program.
     *
     * @param name    Program global name.
     * @param program Program to register.
     *
     * @return
     */
    ViewPtr<program::Program> addProgram(String name, UniquePtr<program::Program> program) override;


    /**
     * @brief Create and register a program.
     *
     * @param name Program name.
     * @param type Program type.
     *
     * @return
     */
    ViewPtr<program::Program> createProgram(String name, StringView type) override;


    /**
     * @brief Remove program from simulation and delete it.
     *
     * @param name Program name.
     */
    void deleteProgram(StringView name) override;


    /**
     * @brief Change simulation world gravity.
     *
     * @param gravity
     */
    void setGravity(const units::AccelerationVector& gravity) noexcept;


#ifdef CECE_ENABLE_BOX2D_PHYSICS

    /**
     * @brief Set physics engine simulation time step.
     *
     * @param dt Time step.
     */
    void setPhysicsEngineTimeStep(units::Time dt) noexcept;

#endif


// Public Operations
public:


    /**
     * @brief Configure simulation.
     *
     * @param config
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Store simulation configuration.
     *
     * @param config
     */
    void storeConfig(config::Configuration& config) const override;


    /**
     * @brief Initialize simulation.
     *
     * @param flag Initialization flag.
     */
    void initialize(AtomicBool& flag) override;


    /**
     * @brief Update simulation.
     *
     * @return If next step can be calculated.
     */
    bool update() override;


    /**
     * @brief Terminate simulation.
     */
    void terminate() override;


    /**
     * @brief Reset simulation.
     */
    bool reset() override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render simulation.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;

#endif


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


// Private Data Members
private:

    /// Simulation plugin context.
    plugin::Context m_pluginContext;

    /// Path of simulation file.
    FilePath m_fileName;

    /// If simulation is initialized.
    bool m_initialized = false;

    /// Number of simulation steps.
    IterationType m_iteration = 0;

    /// Number of iterations.
    IterationType m_iterations = 0;

    /// Simulation step.
    units::Time m_timeStep = Zero;

    /// Total simulation time.
    units::Time m_totalTime = Zero;

    /// World size.
    units::SizeVector m_worldSize{ units::um(400), units::um(400) };

    /// Simulation parameters.
    Parameters m_parameters;

    /// A list of simulation initializers.
    init::Container m_initializers;

    /// Simulation modules.
    module::Container m_modules;

    /// Registered object types.
    object::TypeContainer m_objectTypes;

#ifdef CECE_ENABLE_BOX2D_PHYSICS
    /// Box2D world
    UniquePtr<b2World> m_world;
#endif

    /// Simulation objects.
    object::Container m_objects;

    /// A map of preddefined programs.
    program::NamedContainer m_programs;

#ifdef CECE_ENABLE_RENDER
    /// Simulation visualization.
    Visualization m_visualization;
#endif

#if defined(CECE_ENABLE_RENDER) && defined(CECE_ENABLE_BOX2D_PHYSICS) && defined(CECE_ENABLE_BOX2D_PHYSICS_DEBUG)
    bool m_drawPhysics = false;
#endif

#ifdef CECE_THREAD_SAFE
    Mutex m_mutex;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
