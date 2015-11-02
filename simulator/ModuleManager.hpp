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
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "core/ViewPtr.hpp"
#include "core/UniquePtr.hpp"
#include "core/Map.hpp"
#include "core/Exception.hpp"
#include "simulator/ModuleFactory.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Module;

/* ************************************************************************ */

/**
 * @brief Exception for access to unregistred module factory.
 */
class ModuleFactoryNotFoundException : public InvalidArgumentException
{
    using InvalidArgumentException::InvalidArgumentException;
};

/* ************************************************************************ */

/**
 * @brief Module manager.
 */
class ModuleManager
{

// Public Accessors
public:


    /**
     * @brief Find module factory by name.
     *
     * @param name Factory name.
     *
     * @return
     */
    ViewPtr<ModuleFactory> get(const StringView& name) const noexcept;


// Public Mutators
public:


    /**
     * @brief Register a new factory.
     *
     * @param name    Factory name.
     * @param factory Factory pointer.
     */
    void add(String name, UniquePtr<ModuleFactory> factory)
    {
        m_factories.emplace(std::move(name), std::move(factory));
    }


    /**
     * @brief Register a new factory.
     *
     * @tparam FactoryType
     *
     * @param name Factory name.
     */
    template<typename FactoryType>
    void create(String name)
    {
        add(std::move(name), makeUnique<FactoryType>());
    }


    /**
     * @brief Register a new factory for specified module.
     *
     * @param name Factory name.
     */
    template<typename ModuleType>
    void createForModule(String name)
    {
        create<TypedModuleFactory<ModuleType>>(std::move(name));
    }


// Public Operations
public:


    /**
     * @brief Create module by name.
     *
     * @param name Factory name.
     *
     * @return Created module.
     *
     * @throw ModuleFactoryNotFoundException In case of factory with given name doesn't exists.
     */
    UniquePtr<Module> create(const StringView& name) const;


    /**
     * @brief Returns global instance of module manager.
     *
     * @return
     */
    static ModuleManager& s()
    {
        static ModuleManager instance;
        return instance;
    }


// Private Data Members
private:

    /// Registered module factories.
    Map<String, UniquePtr<ModuleFactory>> m_factories;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
