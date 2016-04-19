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
#include <utility>
#include <algorithm>

// CeCe
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Container for named pointers.
 */
template<typename T>
class PtrNamedContainer
{

// Public Types
public:


    /// Pointer type.
    using type = T;


// Public Structures
public:


    /**
     * @brief Container record.
     */
    struct Record
    {
        /// Object name.
        String name;

        /// Pointer to object.
        UniquePtr<T> object;


        /**
         * @brief Implicit cast to view pointer operator.
         */
        operator ViewPtr<T>() const noexcept
        {
            return object;
        }


        /**
         * @brief Implicit cast to operator.
         */
        operator T*() const noexcept
        {
            return object.get();
        }


        /**
         * @brief Dereference operator.
         *
         * @return Reference.
         */
        T& operator*() const noexcept
        {
            return *object;
        }


        /**
         * @brief Pointer access operator.
         *
         * @return Pointer.
         */
        T* operator->() const noexcept
        {
            return object.get();
        }
    };


// Public Operators
public:


    /**
     * @brief Returns object with given name.
     *
     * @param name Object name.
     *
     * @return Pointer to object.
     */
    ViewPtr<T> operator[](StringView name) const noexcept
    {
        return get(name);
    }


// Public Accessors
public:


    /**
     * @brief Returns a number of stored objects.
     *
     * @return
     */
    std::size_t getCount() const noexcept
    {
        return m_data.size();
    }


    /**
     * @brief Returns if an object with given name exists.
     *
     * @param name Object name.
     *
     * @return
     */
    bool exists(StringView name) const noexcept
    {
        return find(m_data, name) != nullptr;
    }


    /**
     * @brief Returns object with given value.
     *
     * @param name Object name.
     *
     * @return Pointer to object. Can be nullptr.
     */
    ViewPtr<T> get(StringView name) const noexcept
    {
        auto ptr = find(m_data, name);

        if (ptr)
            return *ptr;

        return nullptr;
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    typename DynamicArray<Record>::const_iterator begin() const noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    typename DynamicArray<Record>::const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    typename DynamicArray<Record>::const_iterator end() const noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    typename DynamicArray<Record>::const_iterator cend() const noexcept
    {
        return m_data.cend();
    }


// Public Mutators
public:


    /**
     * @brief Store and object.
     *
     * @param object The object to store.
     *
     * @return View pointer to stored object.
     */
    ViewPtr<T> add(String name, UniquePtr<T> object)
    {
        auto ptr = find(m_data, name);

        if (ptr)
        {
            *ptr = std::move(object);
            return *ptr;
        }
        else
        {
            m_data.emplace_back(Record{std::move(name), std::move(object)});
            return m_data.back().object;
        }
    }


    /**
     * @brief Remove object from container.
     *
     * @param name Object name.
     *
     * @return A pointer to removed object.
     */
    UniquePtr<T> remove(StringView name)
    {
        for (auto itB = m_data.begin(), itE = m_data.end(); itB != itE; ++itB)
        {
            if (itB->name == name)
            {
                auto ptr = std::move(*itB);
                m_data.erase(itB);
                return ptr;
            }
        }

        // Not found
        return nullptr;
    }


    /**
     * @brief Clear container.
     */
    void clear()
    {
        m_data.clear();
    }


// Protected Operations
protected:


    /**
     * @brief Invoke a member function for all stored objects.
     *
     * @tparam Args A list of member function arguments.
     *
     * @param function A pointer to object's member function that will be
     *                 called for all stored listeners.
     * @param args...
     */
    template<typename... Args>
    void invoke(void (T::*fn)(Args...), Args&& ... args) const
    {
        // Foreach all object
        for (const auto& object : m_data)
        {
            // Call member function with given arguments
            (object.object.get()->*fn)(std::forward<Args>(args)...);
        }
    }


// Private Operations
private:


    /**
     * @brief Find an object in container.
     *
     * @param data
     *
     * @return
     */
    template<typename ContainerType>
    static auto find(ContainerType& data, StringView name) noexcept -> decltype(&(data.begin()->object))
    {
        auto it = std::find_if(data.begin(), data.end(),
            [&name](const Record& p) {
                return p.name == name;
            }
        );

        return it != data.end() ? &(it->object) : nullptr;
    }


// Private Data Members
private:

    /// Stored pointers.
    DynamicArray<Record> m_data;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
