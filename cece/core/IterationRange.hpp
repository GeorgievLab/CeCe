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
#include "cece/core/IterationType.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Range of iterations.
 */
class IterationRange
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    IterationRange() = default;


    /**
     * @brief Constructor.
     *
     * @param iteration Single iteration.
     */
    IterationRange(IterationType iteration) noexcept
        : m_first(iteration)
        , m_last(iteration)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param first
     * @param last
     */
    IterationRange(IterationType first, IterationType last) noexcept
        : m_first(first)
        , m_last(last)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns the first iteration number.
     *
     * @return
     */
    IterationType getFirst() const noexcept
    {
        return m_first;
    }


    /**
     * @brief Returns the last iteration number.
     *
     * @return
     */
    IterationType getLast() const noexcept
    {
        return m_last;
    }


    /**
     * @brief Check if given iteration is in range.
     *
     * @param iteration
     *
     * @return
     */
    bool inRange(IterationType iteration) const noexcept
    {
        return iteration >= m_first && iteration <= m_last;
    }


// Private Data Members
private:

    // The first iteration number.
    IterationType m_first{};

    /// The last iteration number.
    IterationType m_last{};
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
