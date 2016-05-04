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
#include "cece/plugins/streamlines/BgkDynamics.hpp"

// C++
#include <iterator>
#include <numeric>

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

BgkDynamics::DensityType
BgkDynamics::computeDensity(const DataType& data) const noexcept
{
    using std::begin;
    using std::end;
    return std::accumulate(begin(data), end(data), DensityType(0.0));
}

/* ************************************************************************ */

BgkDynamics::VelocityType
BgkDynamics::computeVelocity(const DataType& data) const noexcept
{
    return computeMomentum(data) / computeDensity(data);
}

/* ************************************************************************ */

BgkDynamics::MomentumType
BgkDynamics::computeMomentum(const DataType& data) const noexcept
{
    MomentumType momentum = Zero;

    for (Descriptor::DirectionType iPop = 0; iPop < Descriptor::SIZE; ++iPop)
        momentum += Descriptor::DIRECTION_VELOCITIES[iPop] * data[iPop];

    return momentum;
}

/* ************************************************************************ */

BgkDynamics::DensityType
BgkDynamics::computeEquilibrium(DirectionType iPop, DensityType density,
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
BgkDynamics::initEquilibrium(DataType& data, VelocityType velocity, DensityType density) const noexcept
{
    for (Descriptor::DirectionType iPop = 0; iPop < Descriptor::SIZE; ++iPop)
    {
        data[iPop] = computeEquilibrium(iPop, density, velocity);
        Assert(data[iPop] > 0);
    }
}

/* ************************************************************************ */

void
BgkDynamics::defineDensity(DataType& data, DensityType density) const noexcept
{
    initEquilibrium(data, computeVelocity(data), density);
}

/* ************************************************************************ */

void
BgkDynamics::defineVelocity(DataType& data, VelocityType velocity) const noexcept
{
    initEquilibrium(data, velocity, computeDensity(data));
}

/* ************************************************************************ */

void
BgkDynamics::collide(DataType& data) const noexcept
{
    const auto density = computeDensity(data);
    Assert(density > 0);
    const auto velocity = computeVelocity(data);
    const auto omega = getOmega();

    // Perform collision for all populations
    for (Descriptor::DirectionType iPop = 0; iPop < Descriptor::SIZE; ++iPop)
    {
        // Calculate equilibrium distribution
        const auto feq = computeEquilibrium(iPop, density, velocity);

        Assert(feq > 0);
        Assert(data[iPop] > 0);

        // Collide
        data[iPop] += -omega * (data[iPop] - feq);

        // Result value must be positive
        Assert(data[iPop] > 0);
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
