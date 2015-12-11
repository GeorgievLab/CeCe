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

// C++
#include <initializer_list>

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Exception.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Exception for missing parameter.
 */
class MissingParameterException : public InvalidArgumentException
{
    using InvalidArgumentException::InvalidArgumentException;
};

/* ************************************************************************ */

/**
 * @brief Class for storing simulation parameters.
 */
class Parameters
{

// Public Types
public:


    /// Key type.
    using KeyType = String;

    /// Value type.
    using ValueType = RealType;

    /// Key view type.
    using KeyViewType = StringView;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Parameters() = default;


    /**
     * @brief Constructor.
     *
     * @param data Initial data.
     */
    explicit Parameters(std::initializer_list<Pair<KeyType, ValueType>> data)
        : m_data(data)
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Parameter access operator.
     *
     * @param name Parameter name.
     *
     * @return Reference to parameter value.
     *
     * @note In case of missing value, new is created.
     */
    ValueType& operator[](const KeyViewType& name) noexcept
    {
        return get(name);
    }


    /**
     * @brief Parameter access operator.
     *
     * @param name Parameter name.
     *
     * @return Reference to parameter value.
     *
     * @throw MissingParameterException In case of missing value.
     */
    ValueType operator[](const KeyViewType& name) const
    {
        return get(name);
    }


// Public Accessors
public:


    /**
     * @brief Returns if parameter with given name exists.
     *
     * @param name Parameter name.
     *
     * @return
     */
    bool exists(const KeyViewType& name) const noexcept;


    /**
     * @brief Returns parameter with given value.
     *
     * @param name Parameter name.
     *
     * @return Parameter value.
     *
     * @throw MissingParameterException In case of missing value.
     */
    ValueType get(const KeyViewType& name) const;


    /**
     * @brief Returns parameter with given value.
     *
     * @param name Parameter name.
     *
     * @return Parameter value.
     *
     * @throw MissingParameterException In case of missing value.
     */
    ValueType& get(const KeyViewType& name);


    /**
     * @brief Returns parameter with given value.
     *
     * @param name Parameter name.
     * @param def  Default parameter in case of missing value.
     *
     * @return Parameter value.
     */
    ValueType get(const KeyViewType& name, ValueType def) const noexcept;


// Public Mutators
public:


    /**
     * @brief Store or replace parameter.
     *
     * @param name  Parameter name.
     * @param value Value to store.
     */
    void set(KeyType name, ValueType value);


// Public Operations
public:


    /**
     * @brief Merge parameters.
     *
     * @param parameters
     */
    void merge(const Parameters& parameters);


// Private Data Members
private:

    /// Stored data
    DynamicArray<Pair<KeyType, ValueType>> m_data;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
