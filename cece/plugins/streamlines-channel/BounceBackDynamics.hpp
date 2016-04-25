/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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
#include "cece/core/ViewPtr.hpp"

// CeCe Plugins
#include "cece/plugins/streamlines/BounceBackDynamics.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines_channel {

/* ************************************************************************ */

/**
 * @brief Bounce back dynamics.
 */
class BounceBackDynamics : public streamlines::BounceBackDynamics
{

// Public Operations
public:


    /**
     * @brief Initialize node equilibrum.
     *
     * @param density  Macroscopic density.
     * @param iPop     Direction index.
     * @param velocity Macroscopic velocity.
     *
     * @return Equilibrum distribution.
     */
    DensityType computeEquilibrium(DirectionType iPop, DensityType density,
        VelocityType velocity) const noexcept override;


    /**
     * @brief Returns global instance of bounce back dynamics.
     *
     * @return
     */
    static ViewPtr<BounceBackDynamics> getInstance() noexcept;
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
