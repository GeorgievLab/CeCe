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
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/object/Type.hpp"

/* ************************************************************************ */

namespace cece {
namespace object {

/* ************************************************************************ */

/**
 * @brief Container for object types.
 */
class CECE_EXPORT TypeContainer
{


// Public Accessors
public:


    /**
     * @brief Returns if object type with given name exists.
     *
     * @param name ObjectType name.
     *
     * @return
     */
    bool exists(StringView name) const noexcept;


    /**
     * @brief Returns parameter with given value.
     *
     * @param name ObjectType name.
     *
     * @return Pointer to object type. Can be nullptr.
     */
    ViewPtr<const Type> get(StringView name) const noexcept;


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<Type>::const_iterator begin() const noexcept
    {
        return m_types.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<Type>::const_iterator cbegin() const noexcept
    {
        return m_types.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<Type>::const_iterator end() const noexcept
    {
        return m_types.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<Type>::const_iterator cend() const noexcept
    {
        return m_types.cend();
    }


// Public Mutators
public:


    /**
     * @brief Store or replace object type.
     *
     * @param type Object type.
     */
    void add(Type type);


// Private Data Members
private:

    /// Stored types.
    DynamicArray<Type> m_types;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
