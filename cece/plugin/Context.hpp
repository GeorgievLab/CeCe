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
#include "cece/export.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/Parameters.hpp"
#include "cece/loader/FactoryManager.hpp"
#include "cece/init/FactoryManager.hpp"
#include "cece/object/FactoryManager.hpp"
#include "cece/module/FactoryManager.hpp"
#include "cece/program/FactoryManager.hpp"

/* ************************************************************************ */

namespace cece { namespace simulator { class Simulation; } }

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

/**
 * @brief Context for plugins. It stores extensions supplied by plugins.
 */
class CECE_EXPORT Context
{

// Public Accessors
public:


    /**
     * @brief Returns loader factory manager.
     *
     * @return
     */
    loader::FactoryManager& getLoaderFactoryManager() noexcept
    {
        return m_loaderFactoryManager;
    }


    /**
     * @brief Returns init factory manager.
     *
     * @return
     */
    init::FactoryManager& getInitFactoryManager() noexcept
    {
        return m_initFactoryManager;
    }


    /**
     * @brief Returns module factory manager.
     *
     * @return
     */
    module::FactoryManager& getModuleFactoryManager() noexcept
    {
        return m_moduleFactoryManager;
    }


    /**
     * @brief Returns object factory manager.
     *
     * @return
     */
    object::FactoryManager& getObjectFactoryManager() noexcept
    {
        return m_objectFactoryManager;
    }


    /**
     * @brief Returns program factory manager.
     *
     * @return
     */
    program::FactoryManager& getProgramFactoryManager() noexcept
    {
        return m_programFactoryManager;
    }


// Public Mutators
public:


    /**
     * @brief Register new loader type and create factory for it.
     *
     * @tparam LoaderType Type of loader that is created by factory.
     *
     * @param name Factory name.
     */
    template<typename LoaderType>
    void registerLoader(String name) noexcept
    {
        m_loaderFactoryManager.createFor<LoaderType>(std::move(name));
    }


    /**
     * @brief Register new initializer type and create factory for it.
     *
     * @tparam InitializerType Type of initializer that is created by factory.
     *
     * @param name Initializer name.
     */
    template<typename InitializerType>
    void registerInitializer(String name) noexcept
    {
        m_initFactoryManager.createFor<InitializerType>(std::move(name));
    }


    /**
     * @brief Register new module type and create factory for it.
     *
     * @tparam ModuleType Type of module that is created by factory.
     *
     * @param name Factory name.
     */
    template<typename ModuleType>
    void registerModule(String name) noexcept
    {
        m_moduleFactoryManager.createFor<ModuleType>(std::move(name));
    }


    /**
     * @brief Register new object type and create factory for it.
     *
     * @tparam ObjectType Type of object that is created by factory.
     *
     * @param name Factory name.
     */
    template<typename ObjectType>
    void registerObject(String name) noexcept
    {
        m_objectFactoryManager.createFor<ObjectType>(std::move(name));
    }


    /**
     * @brief Register new program type and create factory for it.
     *
     * @tparam ObjectType Type of program that is created by factory.
     *
     * @param name Factory name.
     */
    template<typename ProgramType>
    void registerProgram(String name) noexcept
    {
        m_programFactoryManager.createFor<ProgramType>(std::move(name));
    }


    /**
     * @brief Unregister loader type.
     *
     * @param name Loader type name.
     */
    void unregisterLoader(StringView name) noexcept
    {
        m_loaderFactoryManager.remove(name);
    }


    /**
     * @brief Unregister initializer type.
     *
     * @param name Initializer type name.
     */
    void unregisterInitializer(StringView name) noexcept
    {
        m_initFactoryManager.remove(name);
    }


    /**
     * @brief Unregister module type.
     *
     * @param name Module type name.
     */
    void unregisterModule(StringView name) noexcept
    {
        m_moduleFactoryManager.remove(name);
    }


    /**
     * @brief Unregister object type.
     *
     * @param name Object type name.
     */
    void unregisterObject(StringView name) noexcept
    {
        m_objectFactoryManager.remove(name);
    }


    /**
     * @brief Unregister program type.
     *
     * @param name Program type name.
     */
    void unregisterProgram(StringView name) noexcept
    {
        m_programFactoryManager.remove(name);
    }


// Public Operations
public:


    /**
     * @brief Create a simulation from file.
     *
     * @param filepath   Path to file.
     * @param parameters Optional initialization parameters.
     *
     * @return Pointer to simulation.
     *
     * @throw In case of missing file or error in simulation file.
     */
    UniquePtr<simulator::Simulation> createSimulation(const FilePath& filepath, ViewPtr<const Parameters> parameters = nullptr);


    /**
     * @brief Create a simulation from source.
     *
     * @param type   Type of used loader.
     * @param source Source code.
     *
     * @return Pointer to simulation.
     *
     * @throw In case of missing file or error in simulation file.
     */
    UniquePtr<simulator::Simulation> createSimulation(StringView type, StringView source);


    /**
     * @brief Create an initializer of given type name.
     *
     * @param typeName Type of required initializer.
     *
     * @return Pointer to created initializer.
     */
    UniquePtr<init::Initializer> createInitializer(StringView typeName);


    /**
     * @brief Create a module of given type name.
     *
     * @param typeName   Type of required module.
     * @param simulation Simulation object.
     *
     * @return Pointer to created module.
     */
    UniquePtr<module::Module> createModule(StringView typeName, simulator::Simulation& simulation);


    /**
     * @brief Create an object of given type name.
     *
     * @param typeName   Type of required object.
     * @param simulation Simulation object.
     * @param type       Type of created object.
     *
     * @return Pointer to created object.
     */
    UniquePtr<object::Object> createObject(StringView typeName, simulator::Simulation& simulation, object::Object::Type type);


    /**
     * @brief Create a program of given type name.
     *
     * @param typeName Type of required program.
     *
     * @return Pointer to created program.
     */
    UniquePtr<program::Program> createProgram(StringView typeName);


// Private Data Members
private:

    /// Loader factory manager.
    loader::FactoryManager m_loaderFactoryManager;

    /// Initializer factory manager.
    init::FactoryManager m_initFactoryManager;

    /// Module factory manager.
    module::FactoryManager m_moduleFactoryManager;

    /// Object factory manager.
    object::FactoryManager m_objectFactoryManager;

    /// Program factory manager.
    program::FactoryManager m_programFactoryManager;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
