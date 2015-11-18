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
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/simulator/Program.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Container for programs.
 */
class ProgramContainer
{

// Public Types
public:


    /// Data type.
    using DataType = DynamicArray<Pair<String, Program>>;


// Public Accessors
public:


    /**
     * @brief Returns if program with given name exists.
     *
     * @param name Program name.
     *
     * @return
     */
    bool exists(const StringView& name) const noexcept;


    /**
     * @brief Returns parameter with given value.
     *
     * @param name Program name.
     *
     * @return Pointer to program. Can be nullptr.
     */
    ViewPtr<const Program> get(const StringView& name) const noexcept;


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
     * @brief Store or replace program.
     *
     * @param name    Program name.
     * @param program Program object.
     */
    void add(String name, Program program);


// Private Data Members
private:

    /// Data.
    DataType m_data;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
