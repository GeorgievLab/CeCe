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

namespace cece { namespace simulator { class Simulation; } }

/* ************************************************************************ */

namespace cece {
namespace init {

/* ************************************************************************ */

class Initializer;

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
     * @brief Move assignment operator.
     *
     * @param rhs
     *
     * @return *this
     */
    Container& operator=(Container&& rhs);


    /**
     * @brief Returns n-th initializer.
     *
     * @param pos
     *
     * @return Pointer to initializer.
     */
    ViewPtr<Initializer> operator[](std::size_t pos) const noexcept
    {
        return m_initializers[pos];
    }


// Public Accessors
public:


    /**
     * @brief Returns a number of stored initializers.
     *
     * @return
     */
    std::size_t getCount() const noexcept
    {
        return m_initializers.size();
    }


    /**
     * @brief Returns n-th initializer.
     *
     * @param pos
     *
     * @return Pointer to initializer.
     */
    ViewPtr<Initializer> get(std::size_t pos) const
    {
        return m_initializers.at(pos);
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<UniquePtr<Initializer>>::const_iterator begin() const noexcept
    {
        return m_initializers.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DynamicArray<UniquePtr<Initializer>>::const_iterator cbegin() const noexcept
    {
        return m_initializers.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<UniquePtr<Initializer>>::const_iterator end() const noexcept
    {
        return m_initializers.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DynamicArray<UniquePtr<Initializer>>::const_iterator cend() const noexcept
    {
        return m_initializers.cend();
    }


// Public Mutators
public:


    /**
     * @brief Store an initializer.
     *
     * @param initializer Initializer object.
     *
     * @return View pointer to stored initializer.
     */
    ViewPtr<Initializer> add(UniquePtr<Initializer> initializer)
    {
        m_initializers.push_back(std::move(initializer));
        return m_initializers.back();
    }


    /**
     * @brief Clear container.
     */
    void clear();


// Public Operations
public:


    /**
     * @brief Call all initialzers.
     *
     * @param simulation Simulation object.
     */
    void call(simulator::Simulation& simulation);


// Private Data Members
private:

    /// Stored initializers.
    DynamicArray<UniquePtr<Initializer>> m_initializers;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
