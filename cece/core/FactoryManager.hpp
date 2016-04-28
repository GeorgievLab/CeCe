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

// C++
#include <type_traits>

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/Factory.hpp"

/* ************************************************************************ */

/**
 * @brief Define extern factory manager specialization.
 */
#define CECE_FACTORY_MANAGER(...) \
    namespace cece { inline namespace core { template class CECE_EXPORT FactoryManager<__VA_ARGS__>; } }

/**
 * @brief Define extern factory manager specialization.
 */
#define CECE_FACTORY_MANAGER_EXTERN(...) \
    namespace cece { inline namespace core { extern template class CECE_EXPORT FactoryManager<__VA_ARGS__>; } }

/**
 * @brief Define extern factory manager specialization.
 */
#define CECE_FACTORY_MANAGER_INST(...) \
    namespace cece { inline namespace core { template class FactoryManager<__VA_ARGS__>; } }

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Exception for access to unregistred object factory.
 */
class FactoryNotFoundException : public InvalidArgumentException
{
    using InvalidArgumentException::InvalidArgumentException;
};

/* ************************************************************************ */

/**
 * @brief Basic factory manager.
 *
 * @tparam T Factory type.
 */
template<typename FactoryType>
class FactoryManager
{

// Public Accessors
public:


    /**
     * @brief Find a factory by name.
     *
     * @param name Factory name.
     *
     * @return A pointer to factory or nullptr.
     */
    ViewPtr<FactoryType> get(StringView name) const noexcept
    {
        auto it = m_factories.find(String(name));
        return it != m_factories.end() ? it->second.get() : nullptr;
    }


    /**
     * @brief Returns available factory names.
     *
     * @return
     */
    DynamicArray<String> getNames() const noexcept
    {
        DynamicArray<String> names;

        for (const auto& pair : m_factories)
            names.push_back(pair.first);

        return names;
    }


// Public Mutators
public:


    /**
     * @brief Register a new factory.
     *
     * @param name    Factory name.
     * @param factory Factory pointer.
     */
    void add(String name, UniquePtr<FactoryType> factory) noexcept
    {
        m_factories.emplace(std::move(name), std::move(factory));
    }


    /**
     * @brief Unregister factory.
     *
     * @param name Factory name.
     */
    void remove(StringView name) noexcept
    {
        m_factories.erase(String(name));
    }


    /**
     * @brief Register a new factory.
     *
     * @tparam FactoryTypeInner Type of child factory type.
     *
     * @param name Factory name.
     */
    template<typename FactoryTypeInner>
    void create(String name) noexcept
    {
        static_assert(
            std::is_base_of<FactoryType, FactoryTypeInner>::value,
            "FactoryTypeInner is not a base of FactoryType"
        );

        add(std::move(name), makeUnique<FactoryTypeInner>());
    }


    /**
     * @brief Register a new factory for specified object type.
     *
     * @tparam ObjectType Created object type.
     *
     * @param name Factory name.
     */
    template<typename ObjectType>
    void createFor(String name) noexcept
    {
        create<FactoryTyped<FactoryType, ObjectType>>(std::move(name));
    }


// Protected Operations
protected:


    /**
     * @brief Create object by name.
     *
     * @param name Object name.
     *
     * @return Created object.
     *
     * @throw FactoryNotFoundException In case of factory with given name doesn't exists.
     */
    template<typename T, typename... Args>
    UniquePtr<T> createObject(StringView name, Args&&... args) const
    {
        auto factory = get(name);

        if (factory)
            return factory->create(std::forward<Args>(args)...);

        throw FactoryNotFoundException("Factory not found: " + String(name));
    }


// Private Data Members
private:

    /// Registered factories.
    Map<String, UniquePtr<FactoryType>> m_factories;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
