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

/* ************************************************************************ */

namespace cece {
namespace program {

/* ************************************************************************ */

class Program;

/* ************************************************************************ */

/**
 * @brief Container for named programs.
 */
class NamedContainer
{

// Public Structures
public:


    /**
     * @brief Container record.
     */
    struct Record
    {
        /// Module name.
        String name;

        /// Pointer to program.
        UniquePtr<Program> program;
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~NamedContainer();


// Public Accessors
public:


    /**
     * @brief Returns if program with given name exists.
     *
     * @param name Program name.
     *
     * @return
     */
    bool exists(StringView name) const noexcept;


    /**
     * @brief Returns parameter with given value.
     *
     * @param name Program name.
     *
     * @return Pointer to program. Can be nullptr.
     */
    ViewPtr<Program> get(StringView name) const noexcept;


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<Record>::const_iterator begin() const noexcept
    {
        return m_programs.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<Record>::const_iterator cbegin() const noexcept
    {
        return m_programs.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<Record>::const_iterator end() const noexcept
    {
        return m_programs.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<Record>::const_iterator cend() const noexcept
    {
        return m_programs.cend();
    }


// Public Mutators
public:


    /**
     * @brief Store or replace program.
     *
     * @param name    Program name.
     * @param program Program object.
     */
    void add(String name, UniquePtr<Program> program);


// Private Data Members
private:

    /// Stored programs.
    DynamicArray<Record> m_programs;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
