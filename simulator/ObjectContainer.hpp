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
#include "core/UniquePtr.hpp"
#include "core/ViewPtr.hpp"
#include "core/Pair.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Object.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Container for objects.
 */
class ObjectContainer
{

// Public Types
public:


    /// Data type.
    using DataType = DynamicArray<UniquePtr<Object>>;

    /// Value type.
    using ValueType = DataType::value_type;

    /// Size type.
    using SizeType = DataType::size_type;


// Public Operators
public:


    /**
     * @brief Object access operator.
     *
     * @param pos Object index.
     *
     * @return Pointer to object or nullptr.
     */
    ViewPtr<Object> operator[](SizeType pos) const noexcept
    {
        return get(pos);
    }


// Public Accessors
public:


    /**
     * @brief Returns number of stored objects.
     *
     * @return
     */
    SizeType getCount() const noexcept
    {
        return m_data.size();
    }


    /**
     * @brief Returns parameter with given value.
     *
     * @param pos Object index.
     *
     * @return Pointer to module. Can be nullptr.
     */
    ViewPtr<Object> get(SizeType pos) const noexcept
    {
        if (pos >= getCount())
            return nullptr;

        return m_data[pos];
    }


    /**
     * @brief Get a number of objects that have given type.
     *
     * @param typeName Type name.
     *
     * @return
     */
    SizeType countByType(const StringView& typeName) const noexcept;


    /**
     * @brief Find objects that have given type.
     *
     * @param typeName Type name.
     *
     * @return
     */
    DynamicArray<ViewPtr<Object>> findByType(const StringView& typeName) const noexcept;


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DataType::iterator begin() noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DataType::const_iterator begin() const noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DataType::const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DataType::iterator end() noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DataType::const_iterator end() const noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DataType::const_iterator cend() const noexcept
    {
        return m_data.cend();
    }


// Public Mutators
public:


    /**
     * @brief Store object.
     *
     * @param object Object to store.
     *
     * @return Pointer to added object.
     */
    ViewPtr<Object> add(UniquePtr<Object> object)
    {
        m_data.push_back(std::move(object));
        return m_data.back();
    }


// Private Data Members
private:

    /// Data.
    DataType m_data;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
