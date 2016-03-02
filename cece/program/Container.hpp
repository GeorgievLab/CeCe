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
#include "cece/core/Units.hpp"

/* ************************************************************************ */

namespace cece { namespace object { class Object; } }
namespace cece { namespace simulator { class Simulation; } }

/* ************************************************************************ */

namespace cece {
namespace program {

/* ************************************************************************ */

class Program;

/* ************************************************************************ */

/**
 * @brief Container for programs.
 */
class Container
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    Container() = default;


    /**
     * @brief Copy constructor.
     *
     * @param rhs
     */
    Container(const Container& rhs);


    /**
     * @brief Move constructor.
     *
     * @param rhs
     */
    Container(Container&& rhs) noexcept;


    /**
     * @brief Destructor.
     */
    ~Container();


// Public Operators
public:


    /**
     * @brief Copy assignment operator.
     *
     * @param rhs
     *
     * @return *this
     */
    Container& operator=(const Container& rhs);


    /**
     * @brief MOve assignment operator.
     *
     * @param rhs
     *
     * @return *this
     */
    Container& operator=(Container&& rhs);


    /**
     * @brief Returns n-th program.
     *
     * @param pos
     *
     * @return Pointer to program.
     */
    ViewPtr<Program> operator[](std::size_t pos) const noexcept
    {
        return m_programs[pos];
    }


// Public Accessors
public:


    /**
     * @brief Returns a number of stored programs.
     *
     * @return
     */
    std::size_t getCount() const noexcept
    {
        return m_programs.size();
    }


    /**
     * @brief Returns n-th program.
     *
     * @param pos
     *
     * @return Pointer to program.
     */
    ViewPtr<Program> get(std::size_t pos) const
    {
        return m_programs.at(pos);
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<UniquePtr<Program>>::const_iterator begin() const noexcept
    {
        return m_programs.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<UniquePtr<Program>>::const_iterator cbegin() const noexcept
    {
        return m_programs.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<UniquePtr<Program>>::const_iterator end() const noexcept
    {
        return m_programs.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<UniquePtr<Program>>::const_iterator cend() const noexcept
    {
        return m_programs.cend();
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
        m_programs.push_back(std::move(program));
        return m_programs.back();
    }


    /**
     * @brief Clear container.
     */
    void clear();


// Public Operations
public:


    /**
     * @brief Call all programs.
     *
     * @param simulation Simulation object.
     * @param object     Object.
     * @param dt         Simulation time step.
     */
    void call(simulator::Simulation& simulation, object::Object& object, units::Time dt);


// Private Data Members
private:

    /// Stored programs.
    DynamicArray<UniquePtr<Program>> m_programs;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
