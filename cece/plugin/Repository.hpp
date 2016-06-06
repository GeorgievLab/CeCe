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
#include "cece/core/Assert.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/Pair.hpp"
#include "cece/loader/FactoryManager.hpp"
#include "cece/init/FactoryManager.hpp"
#include "cece/object/FactoryManager.hpp"
#include "cece/module/FactoryManager.hpp"
#include "cece/program/FactoryManager.hpp"
#include "cece/plugin/RepositoryRecord.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {

/* ************************************************************************ */

class Api;
class Manager;

/* ************************************************************************ */

/**
 * @brief Repository of simulation extensions offered by plugins.
 */
class Repository
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param manager Plugins manager.
     */
    explicit Repository(const Manager& manager) noexcept
        : m_manager(manager)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns plugin manager.
     *
     * @return
     */
    const Manager& getManager() const noexcept
    {
        return m_manager;
    }


    /**
     * @brief Returns repository records.
     *
     * @return
     */
    const Map<ViewPtr<const Api>, RepositoryRecord>& getRecords() const noexcept
    {
        return m_records;
    }


    /**
     * @brief Returns if given API has a record in repository.
     *
     * @param api Plugin API.
     *
     * @return
     */
    bool exists(ViewPtr<const Api> api) const noexcept
    {
        auto it = m_records.find(api);
        return it != m_records.end();
    }


    /**
     * @brief Returns repository record for given API.
     *
     * @param api Plugin API.
     *
     * @return Repository record.
     */
    ViewPtr<RepositoryRecord> get(ViewPtr<const Api> api) noexcept
    {
        auto it = m_records.find(api);
        return it != m_records.end() ? &(it->second) : nullptr;
    }


    /**
     * @brief Returns repository record for given API.
     *
     * @param api Plugin API.
     *
     * @return Repository record.
     */
    ViewPtr<const RepositoryRecord> get(ViewPtr<const Api> api) const noexcept
    {
        auto it = m_records.find(api);
        return it != m_records.end() ? &(it->second) : nullptr;
    }


    /**
     * @brief Returns loader factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<loader::FactoryManager> getLoaderFactoryManager(ViewPtr<const Api> api) noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getLoaderFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns loader factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<const loader::FactoryManager> getLoaderFactoryManager(ViewPtr<const Api> api) const noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getLoaderFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns init factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<init::FactoryManager> getInitFactoryManager(ViewPtr<const Api> api) noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getInitFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns init factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<const init::FactoryManager> getInitFactoryManager(ViewPtr<const Api> api) const noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getInitFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns module factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<module::FactoryManager> getModuleFactoryManager(ViewPtr<const Api> api) noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getModuleFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns module factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<const module::FactoryManager> getModuleFactoryManager(ViewPtr<const Api> api) const noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getModuleFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns object factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<object::FactoryManager> getObjectFactoryManager(ViewPtr<const Api> api) noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getObjectFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns object factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<const object::FactoryManager> getObjectFactoryManager(ViewPtr<const Api> api) const noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getObjectFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns program factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<program::FactoryManager> getProgramFactoryManager(ViewPtr<const Api> api) noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getProgramFactoryManager()) : nullptr;
    }


    /**
     * @brief Returns program factory manager for given API.
     *
     * @param api Plugin API.
     *
     * @return Factory manager.
     */
    ViewPtr<const program::FactoryManager> getProgramFactoryManager(ViewPtr<const Api> api) const noexcept
    {
        auto rec = get(api);
        return rec ? &(rec->getProgramFactoryManager()) : nullptr;
    }


// Public Mutators
public:


    /**
     * @brief Register API.
     *
     * @param api Plugin API.
     *
     * @return Repository record.
     */
    RepositoryRecord& registerApi(ViewPtr<const Api> api) noexcept;


    /**
     * @brief Unregister all API factories.
     *
     * @param api Plugin API.
     */
    void unregisterApi(ViewPtr<const Api> api) noexcept;


// Private Data Members
private:

    /// Plugins manager.
    const Manager& m_manager;

    /// Repository records.
    Map<ViewPtr<const Api>, RepositoryRecord> m_records;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
