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
#include "cece/core/StringView.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/object/Object.hpp"

/* ************************************************************************ */

namespace cece {
namespace object {

/* ************************************************************************ */

/**
 * @brief Container for objects.
 */
class CECE_EXPORT Container
{

// Public Structures
public:


    /**
     * @brief Object storage record.
     */
    struct Record
    {
        /// Pointer to object.
        UniquePtr<Object> ptr;

        /// If object has to be deleted.
        bool deleted;


        /**
         * @brief If object is valid.
         *
         * @return
         */
        explicit operator bool() const noexcept
        {
            return !deleted && ptr != nullptr;
        }


        /**
         * @brief Implicit cast to view pointer operator.
         */
        operator ViewPtr<Object>() const noexcept
        {
            return ptr;
        }


        /**
         * @brief Implicit cast to operator.
         */
        operator Object*() const noexcept
        {
            return ptr.get();
        }


        /**
         * @brief Dereference operator.
         *
         * @return Reference.
         */
        Object& operator*() const noexcept
        {
            return *ptr;
        }


        /**
         * @brief Pointer access operator.
         *
         * @return Pointer.
         */
        Object* operator->() const noexcept
        {
            return ptr.get();
        }
    };


// Public Types
public:


    /// Data type.
    using DataType = DynamicArray<Record>;

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

        if (m_data[pos].deleted)
            return nullptr;

        return m_data[pos].ptr;
    }


    /**
     * @brief Get a number of objects that have given type.
     *
     * @param typeName Type name.
     *
     * @return
     */
    SizeType getCountByType(StringView typeName) const noexcept;


    /**
     * @brief Find objects that have given type.
     *
     * @param typeName Type name.
     *
     * @return
     */
    DynamicArray<ViewPtr<Object>> getByType(StringView typeName) const noexcept;


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
    ViewPtr<Object> addObject(UniquePtr<Object> object)
    {
        m_data.push_back(Record{std::move(object), false});
        return m_data.back().ptr;
    }


    /**
     * @brief Mark object as deleted.
     *
     * @param object Object to delete.
     */
    void deleteObject(ViewPtr<Object> object);


// Public Operations
public:


    /**
     * @brief Remove all deleted objects.
     */
    void removeDeleted() noexcept;


// Private Data Members
private:

    /// Data.
    DataType m_data;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
