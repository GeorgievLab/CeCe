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
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

class Program;

/* ************************************************************************ */

/**
 * @brief Container for programs.
 */
class ProgramContainer
{

// Public Types
public:


    /// Data type.
    using DataType = DynamicArray<UniquePtr<Program>>;


// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    ProgramContainer() = default;


    /**
     * @brief Copy constructor.
     *
     * @param rhs
     */
    ProgramContainer(const ProgramContainer& rhs);


    /**
     * @brief Move constructor.
     *
     * @param rhs
     */
    ProgramContainer(ProgramContainer&& rhs);


    /**
     * @brief Destructor.
     */
    ~ProgramContainer();


// Public Operators
public:


    /**
     * @brief Copy assignment operator.
     *
     * @param rhs
     *
     * @return *this
     */
    ProgramContainer& operator=(const ProgramContainer& rhs);


    /**
     * @brief MOve assignment operator.
     *
     * @param rhs
     *
     * @return *this
     */
    ProgramContainer& operator=(ProgramContainer&& rhs);


// Public Accessors
public:


    /**
     * @brief Returns a number of stored programs.
     *
     * @return
     */
    std::size_t getCount() const noexcept
    {
        return m_data.size();
    }


    /**
     * @brief Returns n-th program.
     *
     * @param pos
     *
     * @return Pointer to program.
     */
    ViewPtr<Program> get(std::size_t pos) const noexcept
    {
        return m_data[pos];
    }


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
     * @brief Store a program.
     *
     * @param program Program object.
     *
     * @return View pointer to stored program.
     */
    ViewPtr<Program> add(UniquePtr<Program> program)
    {
        m_data.push_back(std::move(program));
        return m_data.back();
    }


// Private Data Members
private:

    /// Data.
    DataType m_data;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
