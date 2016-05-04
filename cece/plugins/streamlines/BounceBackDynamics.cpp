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
#include "cece/plugins/streamlines/BounceBackDynamics.hpp"

// C++
#include <utility>
#include <iterator>
#include <numeric>

// CeCe
#include "cece/core/Zero.hpp"

// Plugin
#include "cece/plugins/streamlines/config.hpp"
#include "cece/plugins/streamlines/Descriptor.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

BounceBackDynamics::DensityType
BounceBackDynamics::computeDensity(const DataType& data) const noexcept
{
    using std::begin;
    using std::end;
    return std::accumulate(begin(data), end(data), DensityType(0.0));
}

/* ************************************************************************ */

BounceBackDynamics::VelocityType
BounceBackDynamics::computeVelocity(const DataType& data) const noexcept
{
    return Zero;
}

/* ************************************************************************ */

BounceBackDynamics::MomentumType
BounceBackDynamics::computeMomentum(const DataType& data) const noexcept
{
    return Zero;
}

/* ************************************************************************ */

BounceBackDynamics::DensityType
BounceBackDynamics::computeEquilibrium(DirectionType iPop, DensityType density,
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
BounceBackDynamics::initEquilibrium(DataType& data, VelocityType velocity, DensityType density) const noexcept
{
    for (Descriptor::DirectionType iPop = 0; iPop < Descriptor::SIZE; ++iPop)
        data[iPop] = computeEquilibrium(iPop, density, velocity);
}

/* ************************************************************************ */

void
BounceBackDynamics::defineDensity(DataType& data, DensityType density) const noexcept
{
    // Nothing to do
}

/* ************************************************************************ */

void
BounceBackDynamics::defineVelocity(DataType& data, VelocityType velocity) const noexcept
{
    // Nothing to do
}

/* ************************************************************************ */

void
BounceBackDynamics::collide(DataType& data) const noexcept
{
#ifdef CECE_PLUGIN_streamlines_SWAP_TRICK
    constexpr Descriptor::DirectionType half = (Descriptor::SIZE - 1) / 2;

    // Swap opposite (without temporary memory)
    for (Descriptor::DirectionType i = 1; i <= half; ++i)
    {
        using std::swap;
        swap(data[i], data[i + half]);
    }
#else
    // Static obstacle, bounce all back
    DataType temp;

    // Move updated values into opposite directions
    for (Descriptor::DirectionType iPop = 0; iPop < Descriptor::SIZE; ++iPop)
        temp[iPop] = data[Descriptor::DIRECTION_OPPOSITES[iPop]];

    // Copy updated values
    data = temp;
#endif
}

/* ************************************************************************ */

ViewPtr<BounceBackDynamics> BounceBackDynamics::getInstance() noexcept
{
    static BounceBackDynamics instance;
    return &instance;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
