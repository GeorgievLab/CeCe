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
#include "cece/simulator/IterationType.hpp"

/* ************************************************************************ */

namespace cece { namespace init { class Initializer; } }
namespace cece { namespace module { class Module; } }
namespace cece { namespace object { class Object; } }
namespace cece { namespace object { class Type; } }
namespace cece { namespace program { class Program; } }
namespace cece { namespace config { class Configuration; } }
namespace cece { namespace plugin { class Api; } }

#ifdef CECE_ENABLE_RENDER
namespace cece { namespace render { class Context; } }
namespace cece { namespace simulator { class Visualization; } }
#endif

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
     * @brief Virtual destructor.
     */
    virtual ~Simulation();


// Public Accessors
public:


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
    virtual const SizeVector& getWorldSize() const noexcept = 0;


    /**
     * @brief Returns if simulation is initialized.
     *
     * @return
     */
    virtual bool isInitialized() const noexcept = 0;


    /**
     * @brief Returns current iteration.
     *
     * @return
     */
    virtual IterationNumber getIteration() const noexcept = 0;


    /**
     * @brief Returns a number of total iterations.
     *
     * @return
     */
    virtual IterationNumber getIterations() const noexcept = 0;


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
    virtual Visualization& getVisualization() noexcept;


    /**
     * @brief Returns visualization
     *
     * @return
     */
    virtual const Visualization& getVisualization() const noexcept = 0;

#endif


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
    virtual const String& getParameter(StringView name) const = 0;


    /**
     * @brief Returns if module with given name exists.
     *
     * @param name Module name.
     *
     * @return
     */
    virtual bool hasModule(StringView name) const noexcept = 0;


    /**
     * @brief Returns required module.
     *
     * @param name Module name.
     *
     * @return
     */
    virtual ViewPtr<module::Module> getModule(StringView name) const noexcept = 0;


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
    virtual ViewPtr<module::Module> requireModule(StringView name) const;


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


    /**
     * @brief Check if program with given name exists.
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
     * @return Pointer to program.
     */
    virtual UniquePtr<program::Program> getProgram(StringView name) const = 0;


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
    virtual ViewPtr<plugin::Api> loadPlugin(StringView name) = 0;


    /**
     * @brief Load plugin into simulation. Loaded plugins allow to use their
     * symbols within simulation.
     *
     * @param config Plugin configuration.
     *
     * @return Plugin API.
     */
    virtual ViewPtr<plugin::Api> loadPlugin(const config::Configuration& config);


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
    virtual void setIterations(IterationNumber iterations) noexcept = 0;


    /**
     * @brief Change world size.
     *
     * @param size Simulation world size.
     */
    virtual void setWorldSize(SizeVector size) noexcept = 0;


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
     * @return
     */
    virtual ViewPtr<init::Initializer> addInitializer(UniquePtr<init::Initializer> initializer) = 0;


    /**
     * @brief Create and register initializer.
     *
     * @param type Initializer type.
     *
     * @return
     */
    virtual ViewPtr<init::Initializer> createInitializer(StringView type) = 0;


    /**
     * @brief Create, register and configure initializer.
     *
     * @param config Initializer configuration.
     *
     * @return
     */
    virtual ViewPtr<init::Initializer> createInitializer(const config::Configuration& config);


    /**
     * @brief Remove and delete initializer.
     *
     * @param initializer Initializer to delete.
     *
     * @return
     */
    virtual void deleteInitializer(ViewPtr<init::Initializer> initializer) = 0;


    /**
     * @brief Add a module.
     *
     * @param name   Module name.
     * @param module Added module.
     *
     * @return
     */
    virtual ViewPtr<module::Module> addModule(String name, UniquePtr<module::Module> module) = 0;


    /**
     * @brief Create and register a module.
     *
     * @param type Module type and registration name.
     *
     * @return
     */
    virtual ViewPtr<module::Module> createModule(StringView type) = 0;


    /**
     * @brief Create and register a module.
     *
     * @param config Module configuration.
     *
     * @return
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
     * @param name Object type name.
     * @param type Objec type.
     *
     * @return
     */
    virtual ViewPtr<object::Type> addObjectType(String name, UniquePtr<object::Type> type) = 0;


    /**
     * @brief Create and register object type.
     *
     * @param name Object type name.
     *
     * @return
     */
    virtual ViewPtr<object::Type> createObjectType(String name) = 0;


    /**
     * @brief Create, register and configure object type.
     *
     * @param config Object type configuration.
     *
     * @return
     */
    virtual ViewPtr<object::Type> createObjectType(const config::Configuration& config);


    /**
     * @brief Remove and delete and object type.
     *
     * @param name Object type name.
     *
     * @return
     */
    virtual void deleteObjectType(StringView name) = 0;


    /**
     * @brief Add an object.
     *
     * @param object Added object.
     *
     * @return
     */
    virtual ViewPtr<object::Object> addObject(UniquePtr<object::Object> object) = 0;


    /**
     * @brief Create and register an object.
     *
     * @param type Object type.
     *
     * @return
     */
    virtual ViewPtr<object::Object> createObject(StringView type) = 0;


    /**
     * @brief Create, register and configure an object.
     *
     * @param config Object configuration.
     *
     * @return
     */
    virtual ViewPtr<object::Object> createObject(const config::Configuration& config);


    /**
     * @brief Remove given object from simulation and delete it.
     *
     * @param object Pointer to deleted object.
     */
    virtual void deleteObject(ViewPtr<object::Object> object) = 0;


    /**
     * @brief Register a program.
     *
     * @param name    Program global name.
     * @param program Program to register.
     *
     * @return
     */
    virtual ViewPtr<program::Program> addProgram(String name, UniquePtr<program::Program> program) = 0;


    /**
     * @brief Create and register a program.
     *
     * @param name Program name.
     * @param type Program type.
     *
     * @return
     */
    virtual ViewPtr<program::Program> createProgram(String name, StringView type) = 0;


    /**
     * @brief Create, register and configure a program.
     *
     * @param config Program configuration.
     *
     * @return
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
     * @param config
     */
    virtual void loadConfig(const config::Configuration& config);


    /**
     * @brief Store simulation configuration.
     *
     * @param config
     */
    virtual void storeConfig(config::Configuration& config) const;


    /**
     * @brief Initialize simulation.
     *
     * @param flag Initialization flag.
     */
    virtual void initialize(AtomicBool& flag) = 0;


    /**
     * @brief Update simulation.
     *
     * @return If next step can be calculated.
     */
    virtual bool update() = 0;


    /**
     * @brief Terminate simulation.
     */
    virtual void terminate() = 0;


    /**
     * @brief Reset simulation.
     */
    virtual void reset() = 0;


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
