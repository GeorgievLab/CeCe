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
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/loader/FactoryManager.hpp"
#include "cece/init/FactoryManager.hpp"
#include "cece/object/FactoryManager.hpp"
#include "cece/module/FactoryManager.hpp"
#include "cece/program/FactoryManager.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

/**
 * @brief Repository record.
 */
class RepositoryRecord
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
     * @brief Returns loader factory manager.
     *
     * @return
     */
    const loader::FactoryManager& getLoaderFactoryManager() const noexcept
    {
        return m_loaderFactoryManager;
    }


    /**
     * @brief Returns initializer factory manager.
     *
     * @return
     */
    init::FactoryManager& getInitFactoryManager() noexcept
    {
        return m_initFactoryManager;
    }


    /**
     * @brief Returns initializer factory manager.
     *
     * @return
     */
    const init::FactoryManager& getInitFactoryManager() const noexcept
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
     * @brief Returns module factory manager.
     *
     * @return
     */
    const module::FactoryManager& getModuleFactoryManager() const noexcept
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
     * @brief Returns object factory manager.
     *
     * @return
     */
    const object::FactoryManager& getObjectFactoryManager() const noexcept
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


    /**
     * @brief Returns program factory manager.
     *
     * @return
     */
    const program::FactoryManager& getProgramFactoryManager() const noexcept
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
     *
     * @return *this.
     */
    template<typename LoaderType>
    RepositoryRecord& registerLoader(String name) noexcept
    {
        m_loaderFactoryManager.createFor<LoaderType>(std::move(name));
        return *this;
    }


    /**
     * @brief Register new initializer type and create factory for it.
     *
     * @tparam InitializerType Type of initializer that is created by factory.
     *
     * @param name Initializer name.
     *
     * @return *this.
     */
    template<typename InitializerType>
    RepositoryRecord& registerInitializer(String name) noexcept
    {
        m_initFactoryManager.createFor<InitializerType>(std::move(name));
        return *this;
    }


    /**
     * @brief Register new module type and create factory for it.
     *
     * @tparam ModuleType Type of module that is created by factory.
     *
     * @param name Factory name.
     *
     * @return *this.
     */
    template<typename ModuleType>
    RepositoryRecord& registerModule(String name) noexcept
    {
        m_moduleFactoryManager.createFor<ModuleType>(std::move(name));
        return *this;
    }


    /**
     * @brief Register new object type and create factory for it.
     *
     * @tparam ObjectType Type of object that is created by factory.
     *
     * @param name Factory name.
     *
     * @return *this.
     */
    template<typename ObjectType>
    RepositoryRecord& registerObject(String name) noexcept
    {
        m_objectFactoryManager.createFor<ObjectType>(std::move(name));
        return *this;
    }


    /**
     * @brief Register new program type and create factory for it.
     *
     * @tparam ObjectType Type of program that is created by factory.
     *
     * @param name Factory name.
     *
     * @return *this.
     */
    template<typename ProgramType>
    RepositoryRecord& registerProgram(String name) noexcept
    {
        m_programFactoryManager.createFor<ProgramType>(std::move(name));
        return *this;
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
