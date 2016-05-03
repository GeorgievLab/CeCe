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

// Declaration
#include "cece/plugins/streamlines/NoDynamics.hpp"

// Plugin
#include "cece/plugins/streamlines/Descriptor.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

NoDynamics::DensityType
NoDynamics::computeEquilibrium(DirectionType iPop, DensityType density,
    VelocityType velocity) const noexcept
{
    return Descriptor::calcEquilibrium(
        Descriptor::DIRECTION_WEIGHTS[iPop],
        Descriptor::DIRECTION_VELOCITIES[iPop],
        density,
        velocity
    );
}

/* ************************************************************************ */

void
NoDynamics::initEquilibrium(DataType& data, VelocityType velocity, DensityType density) const noexcept
{
    for (Descriptor::DirectionType iPop = 0; iPop < Descriptor::SIZE; ++iPop)
        data[iPop] = computeEquilibrium(iPop, density, velocity);
}

/* ************************************************************************ */

ViewPtr<NoDynamics> NoDynamics::getInstance() noexcept
{
    static NoDynamics instance;
    return &instance;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
