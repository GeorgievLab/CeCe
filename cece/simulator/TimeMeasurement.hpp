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
#include "cece/core/ViewPtr.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/TimeMeasurement.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Time measurement functor with printing current iteration.
 */
struct CECE_EXPORT TimeMeasurement
{
    /// Simulation.
    ViewPtr<const Simulation> m_simulation;


    /**
     * @brief Constructor.
     *
     * @param sim Simulation.
     */
    explicit TimeMeasurement(ViewPtr<const Simulation> sim)
        : m_simulation(sim)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param sim Simulation
     */
    explicit TimeMeasurement(const Simulation& sim)
        : m_simulation(&sim)
    {
        // Nothing to do
    }


    /**
     * @brief Functor function.
     *
     * @param out  Output stream.
     * @param name Measurement name.
     * @param dt   Measured time.
     */
    void operator()(OutStream& out, StringView name, Clock::duration dt) const noexcept;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
