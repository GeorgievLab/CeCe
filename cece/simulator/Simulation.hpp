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

// CeCe
#include "cece/config.hpp"
#include "cece/core/Atomic.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/InOutStream.hpp"
#include "cece/core/IterationType.hpp"

/// @deprecated
#include "cece/object/Object.hpp"

/* ************************************************************************ */

namespace cece {
    /// @deprecated
    inline namespace core { class Parameters; }

    namespace config { class Configuration; }
    namespace plugin { class Api; }
    namespace init { class Initializer; }
    namespace module { class Module; }
    //namespace object { class Object; }
    namespace object { class Type; }
    namespace program { class Program; }

#ifdef CECE_ENABLE_RENDER
    namespace render { class Context; }
    namespace simulator { class Visualization; }
#endif
}

#ifdef CECE_ENABLE_BOX2D_PHYSICS
/// @deprecated
class b2World;
#endif

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Abstract simulation class.
 */
class Simulation
{

// Public Ctors
public:


    /**
     * @brief Virtual destructor.
     */
    virtual ~Simulation();


// Public Accessors
public:


    /**
     * @brief Returns simulation parameters.
     *
     * @return
     * @deprecated
     */
    virtual Parameters& getParameters() noexcept = 0;


    /**
     * @brief Returns simulation parameters.
     *
     * @return
     * @deprecated
     */
    virtual const Parameters& getParameters() const noexcept = 0;


    /**
     * @brief Returns simulation file name.
     *
     * @return
     */
    virtual const FilePath& getFileName() const noexcept = 0;


    /**
     * @brief Returns world size.
     *
     * @return
     */
    virtual const units::SizeVector& getWorldSize() const noexcept = 0;


    /**
     * @brief Returns current iteration.
     *
     * @return
     */
    virtual IterationType getIteration() const noexcept = 0;


    /**
     * @brief Returns a number of total iterations.
     *
     * @return
     */
    virtual IterationType getIterations() const noexcept = 0;


    /**
     * @brief Returns if there are unlimited iterations.
     *
     * @return
     */
    virtual bool hasUnlimitedIterations() const noexcept
    {
        return getIterations() == 0;
    }


    /**
     * @brief Returns current simulation time step.
     *
     * @return
     */
    virtual units::Time getTimeStep() const noexcept = 0;


    /**
     * @brief Returns total simulation time.
     *
     * @return
     */
    virtual units::Time getTotalTime() const noexcept = 0;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Returns visualization
     *
     * @return
     */
    virtual Visualization& getVisualization() noexcept = 0;


    /**
     * @brief Returns visualization
     *
     * @return
     */
    virtual const Visualization& getVisualization() const noexcept = 0;

#endif


    /**
     * @brief Returns if simulation is initialized.
     *
     * @return
     */
    virtual bool isInitialized() const noexcept = 0;


    /**
     * @brief Returns simulation resource.
     *
     * @param name Resource name.
     *
     * @return Pointer to resource stream or nullptr.
     */
    virtual UniquePtr<InOutStream> getResource(StringView name) noexcept = 0;


    /**
     * @brief Returns if simulation parameter exists.
     *
     * @param name Parameter name.
     *
     * @return
     */
    virtual bool hasParameter(StringView name) const noexcept = 0;


    /**
     * @brief Returns simulation parameter.
     *
     * @param name Parameter name.
     *
     * @return Parameter value.
     *
     * @throw RuntimeException
     */
    virtual String getParameter(StringView name) const = 0;


    /**
     * @brief Returns simulation parameter.
     *
     * @param name Parameter name.
     * @param def  Default parameter value.
     *
     * @return Parameter value.
     */
    virtual String getParameter(StringView name, String def) const
    {
        return hasParameter(name) ? getParameter(name) : def;
    }


    /**
     * @brief Returns if module with given name exists.
     *
     * @param name Module name.
     *
     * @return
     */
    virtual bool hasModule(StringView name) const noexcept = 0;


    /**
     * @brief Returns module stored under given name.
     *
     * @param name Module name.
     *
     * @return Pointer to module or nullptr.
     */
    virtual ViewPtr<module::Module> getModule(StringView name) const noexcept = 0;


    /**
     * @brief Returns module stored under given name.
     *
     * @tparam ModuleType Type of required module.
     *
     * @param name Module name.
     *
     * @return Pointer to module or nullptr.
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
     * @brief Returns module or throw if module not found.
     *
     * @param name Module name.
     *
     * @return Pointer to module.
     *
     * @throw RuntimeException In case module is not found.
     */
    virtual ViewPtr<module::Module> requireModule(StringView name) const;


    /**
     * @brief Returns module or throw if module not found.
     *
     * @tparam ModuleType Type of required module.
     *
     * @param name Module name.
     *
     * @return Pointer to module.
     *
     * @throw RuntimeException In case module is not found.
     */
    template<typename ModuleType>
    ViewPtr<ModuleType> requireModule(StringView name) const
    {
        auto module = requireModule(name);
        Assert(dynamic_cast<ModuleType*>(module.get()));
        return static_cast<ModuleType*>(module.get());
    }


    /**
     * @brief Check if global program with given name exists.
     *
     * @param name Program name.
     *
     * @return
     */
    virtual bool hasProgram(StringView name) const noexcept = 0;


    /**
     * @brief Returns program with given name. At first global storage of
     * program checked and then program factory. In case of global storage
     * a copy is created so each object work with own data.
     *
     * @param name Program name.
     *
     * @return Pointer to program or nullptr.
     */
    virtual UniquePtr<program::Program> getProgram(StringView name) const = 0;


    /**
     * @brief Returns program with given name. At first global storage of
     * program checked and then program factory. In case of global storage
     * a copy is created so each object work with own data.
     *
     * @param name Program name.
     *
     * @return Pointer to program.
     *
     * @throw RuntimeException In case program is not found.
     */
    virtual UniquePtr<program::Program> requireProgram(StringView name) const;


    /**
     * @brief Returns number of simulation objects.
     *
     * @return
     */
    virtual std::size_t getObjectCount() const noexcept = 0;


    /**
     * @brief Returns number of simulation objects with given type.
     *
     * @param type Object type.
     *
     * @return
     */
    virtual std::size_t getObjectCount(StringView type) const noexcept;


    /**
     * @brief Returns all objects.
     *
     * @return
     */
    virtual DynamicArray<ViewPtr<object::Object>> getObjects() const noexcept = 0;


    /**
     * @brief Returns all objects with given type.
     *
     * @param type Object type.
     *
     * @return
     */
    virtual DynamicArray<ViewPtr<object::Object>> getObjects(StringView type) const noexcept;


#ifdef CECE_ENABLE_BOX2D_PHYSICS

    /**
     * @brief Returns physics world.
     *
     * @return
     * @deprecated
     */
    virtual b2World& getWorld() noexcept = 0;


    /**
     * @brief Returns physics world.
     *
     * @return
     * @deprecated
     */
    virtual const b2World& getWorld() const noexcept = 0;

#endif


    /**
     * @brief Returns maximum translation vector magnitude per iteration.
     *
     * @return
     * @deprecated
     */
    virtual units::Length getMaxObjectTranslation() const noexcept = 0;


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
    virtual ViewPtr<const plugin::Api> loadPlugin(StringView name) = 0;


    /**
     * @brief Load plugin into simulation. Loaded plugins allow to use their
     * symbols within simulation.
     *
     * @param config Plugin configuration.
     *
     * @return Plugin API.
     */
    virtual ViewPtr<const plugin::Api> loadPlugin(const config::Configuration& config);


    /**
     * @brief Unload plugin from simulation.
     *
     * @param name Plugin name.
     */
    virtual void unloadPlugin(StringView name) = 0;


    /**
     * @brief Set total number of simulation iterations.
     *
     * @param iterations Number of iterations.
     */
    virtual void setIterations(IterationType iterations) noexcept = 0;


    /**
     * @brief Change world size.
     *
     * @param size Simulation world size.
     */
    virtual void setWorldSize(units::SizeVector size) noexcept = 0;


    /**
     * @brief Set simulation time step.
     *
     * @param dt Time step.
     */
    virtual void setTimeStep(units::Time dt) = 0;


    /**
     * @brief Set simulation parameter.
     *
     * @param name  Parameter name.
     * @param value Parameter value.
     */
    virtual void setParameter(String name, String value) = 0;


    /**
     * @brief Add an initializer.
     *
     * @param initializer Initializer.
     *
     * @return Added initializer.
     */
    virtual ViewPtr<init::Initializer> addInitializer(UniquePtr<init::Initializer> initializer) = 0;


    /**
     * @brief Create an initializer.
     *
     * @return Pointer to created initializer.
     */
    template<typename T>
    ViewPtr<T> createInitializer()
    {
        return addInitializer(makeUnique<T>());
    }


    /**
     * @brief Create and register initializer.
     *
     * @param type Initializer type.
     *
     * @return Pointer to created initializer.
     */
    virtual ViewPtr<init::Initializer> createInitializer(StringView type) = 0;


    /**
     * @brief Create, register and configure initializer.
     *
     * @param config Initializer configuration.
     *
     * @return Pointer to created initializer.
     */
    virtual ViewPtr<init::Initializer> createInitializer(const config::Configuration& config);


    /**
     * @brief Remove and delete initializer.
     *
     * @param initializer Initializer to delete.
     */
    virtual void deleteInitializer(ViewPtr<init::Initializer> initializer) = 0;


    /**
     * @brief Adds a module.
     *
     * @param name   Module name.
     * @param module Module to add.
     *
     * @return Pointer to added module.
     */
    virtual ViewPtr<module::Module> addModule(String name, UniquePtr<module::Module> module) = 0;


    /**
     * @brief Create module.
     *
     * @tparam ModuleType Created module type.
     *
     * @param name Module name.
     *
     * @return Pointer to created module.
     */
    template<typename ModuleType>
    ViewPtr<ModuleType> createModule(String name)
    {
        return addModule(std::move(name), makeUnique<ModuleType>(*this));
    }


    /**
     * @brief Create and register module.
     *
     * @param type Module type and registration name.
     *
     * @return Pointer to created module.
     */
    virtual ViewPtr<module::Module> createModule(StringView type) = 0;


    /**
     * @brief Create, register and configure module.
     *
     * @param config Module configuration.
     *
     * @return Pointer to created module.
     */
    virtual ViewPtr<module::Module> createModule(const config::Configuration& config);


    /**
     * @brief Remove and delete module.
     *
     * @param name Module name.
     */
    virtual void deleteModule(StringView name) = 0;


    /**
     * @brief Register object type.
     *
     * @param name   Object type name.
     * @param parent Parent type name.
     * @param config Type configuration.
     */
    virtual void addObjectType(String name, String parent, const config::Configuration& config) = 0;


    /**
     * @brief Create, register and configure object type.
     *
     * @param config Type configuration.
     */
    virtual void createObjectType(const config::Configuration& config);


    /**
     * @brief Adds an object.
     *
     * @param object Object to add.
     *
     * @return Pointer to added object.
     */
    virtual ViewPtr<object::Object> addObject(UniquePtr<object::Object> object) = 0;


    /**
     * @brief Create object.
     *
     * @tparam ObjectType Type of created object.
     *
     * @return Created object
     */
    template<typename ObjectType>
    ViewPtr<ObjectType> createObject()
    {
        return addObject(makeUnique<ObjectType>(*this));
    }


    /**
     * @brief Create and register object.
     *
     * @param type Object type name.
     *
     * @return Pointer to created object.
     */
    virtual ViewPtr<object::Object> createObject(StringView type) = 0;


    /**
     * @brief Create and register object.
     *
     * @param type Object type name.
     *
     * @return Pointer to created object.
     * @deprecated
     */
    virtual ViewPtr<object::Object> createObject(StringView type, object::Object::Type state) = 0;


    /**
     * @brief Create, register and configure object.
     *
     * @param config Object configuration.
     *
     *
     * @return Pointer to created object.
     */
    virtual ViewPtr<object::Object> createObject(const config::Configuration& config);


    /**
     * @brief Remove given object from simulation and delete it.
     *
     * @param object Pointer to deleted object.
     */
    virtual void deleteObject(ViewPtr<object::Object> object) = 0;


    /**
     * @brief Adds a program.
     *
     * @param name    Program global name.
     * @param program Program to register.
     *
     * @return Pointer to added program.
     */
    virtual ViewPtr<program::Program> addProgram(String name, UniquePtr<program::Program> program) = 0;


    /**
     * @brief Create and register program.
     *
     * @param name Program name.
     * @param type Program type.
     *
     * @return Pointer to created program.
     */
    virtual ViewPtr<program::Program> createProgram(String name, StringView type) = 0;


    /**
     * @brief Create, register and configure program.
     *
     * @param config Program configuration.
     *
     * @return Pointer to created program.
     */
    virtual ViewPtr<program::Program> createProgram(const config::Configuration& config);


    /**
     * @brief Remove program from simulation and delete it.
     *
     * @param name Program name.
     */
    virtual void deleteProgram(StringView name) = 0;


// Public Operations
public:


    /**
     * @brief Configure simulation.
     *
     * @param config Source configuration.
     */
    virtual void loadConfig(const config::Configuration& config);


    /**
     * @brief Store simulation configuration.
     *
     * @param config Output configuration.
     */
    virtual void storeConfig(config::Configuration& config) const;


    /**
     * @brief Initialize simulation.
     *
     * @param flag Initialization flag.
     */
    virtual void initialize(AtomicBool& flag) = 0;


    /**
     * @brief Perform simulation update.
     *
     * @return If simulation is not finished.
     */
    virtual bool update() = 0;


    /**
     * @brief Terminate simulation.
     */
    virtual void terminate() = 0;


    /**
     * @brief Reset simulation.
     *
     * Some simulation implementation cannot support reset feature because
     * it's not possible to restore simulation initial state (objects, modules, etc.)
     *
     * @return If simulation reset is supported.
     */
    virtual bool reset() = 0;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render simulation.
     *
     * @param context Rendering context.
     */
    virtual void draw(render::Context& context) = 0;

#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
