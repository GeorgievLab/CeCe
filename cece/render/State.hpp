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

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/render/Context.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief Special template class for storing renderable objects state.
 *
 * @tparam StateType State type.
 */
template<typename StateType>
class State
{

// Public Accessors & Mutators
public:


    /**
     * @brief Returns front state.
     *
     * @return
     */
    StateType& getFront() noexcept
    {
        return m_front;
    }


    /**
     * @brief Returns front state.
     *
     * @return
     */
    const StateType& getFront() const noexcept
    {
        return m_front;
    }


    /**
     * @brief Returns back state.
     *
     * @return
     */
    StateType& getBack() noexcept
    {
        return m_back;
    }


    /**
     * @brief Returns back state.
     *
     * @return
     */
    const StateType& getBack() const noexcept
    {
        return m_back;
    }


// Public Operations
public:


    /**
     * @brief Swap states (front & back).
     */
    void swap()
    {
        using std::swap;
        swap(m_front, m_back);
    }


// Private Data Members
private:

    /// Currently rendered state.
    StateType m_front;

    /// Currently written state.
    StateType m_back;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
