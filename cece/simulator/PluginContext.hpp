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

// CeCe
#include "cece/simulator/ObjectFactoryManager.hpp"
#include "cece/simulator/ModuleFactoryManager.hpp"
#include "cece/loader/FactoryManager.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Context for plugins. It stores extensions supplied by plugins.
 */
class PluginContext
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
     * @brief Returns module factory manager.
     *
     * @return
     */
    ModuleFactoryManager& getModuleFactoryManager() noexcept
    {
        return m_moduleFactoryManager;
    }


    /**
     * @brief Returns object factory manager.
     *
     * @return
     */
    ObjectFactoryManager& getObjectFactoryManager() noexcept
    {
        return m_objectFactoryManager;
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
        m_loaderFactoryManager.createForLoader<LoaderType>(std::move(name));
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
        m_moduleFactoryManager.createForModule<ModuleType>(std::move(name));
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
        m_objectFactoryManager.createForObject<ObjectType>(std::move(name));
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


// Private Data Members
private:

    /// Loader factory manager.
    loader::FactoryManager m_loaderFactoryManager;

    /// Module factory manager.
    ModuleFactoryManager m_moduleFactoryManager;

    /// Object factory manager.
    ObjectFactoryManager m_objectFactoryManager;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
