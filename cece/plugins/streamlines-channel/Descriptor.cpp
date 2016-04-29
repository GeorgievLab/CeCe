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

// Declaration
#include "cece/plugins/streamlines-channel/Descriptor.hpp"

// C++
#include <iterator>
#include <numeric>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines_channel {

/* ************************************************************************ */

RealType Descriptor::s_splitCoefficient = 1.0;

/* ************************************************************************ */

StaticArray<RealType, Descriptor::SIZE> Descriptor::s_weightsHorizontal{{
    WEIGHT_CENTER, // Center
    WEIGHT_DIAGONAL,
    WEIGHT_LINEAR,
    WEIGHT_DIAGONAL,
    WEIGHT_LINEAR,
    WEIGHT_DIAGONAL,
    WEIGHT_LINEAR,
    WEIGHT_DIAGONAL,
    WEIGHT_LINEAR
}};

/* ************************************************************************ */

StaticArray<RealType, Descriptor::SIZE> Descriptor::s_weightsVertical{{
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0
}};

/* ************************************************************************ */

RealType Descriptor::getWeightHorizontalSum() noexcept
{
    using std::begin;
    using std::end;

    return std::accumulate(begin(s_weightsHorizontal), end(s_weightsHorizontal), RealType(0.0));
}

/* ************************************************************************ */

RealType Descriptor::getWeightVerticalSum() noexcept
{
    using std::begin;
    using std::end;

    return std::accumulate(begin(s_weightsVertical), end(s_weightsVertical), RealType(0.0));
}

/* ************************************************************************ */

void Descriptor::initModel(RealType height)
{
    if (height <= 0)
        throw InvalidArgumentException("Negative or zero height doesn't make sense");

    const auto heightSq = (0.25 * height) * (0.25 * height);

    if (heightSq <= SPEED_OF_SOUND_SQ)
        throw InvalidArgumentException("Channel height is too small");

    // Calculate split coefficient
    s_splitCoefficient = 1.0 - SPEED_OF_SOUND_SQ / heightSq;

    for (std::size_t i = 0; i < SIZE; ++i)
    {
        const auto weight = s_weightsHorizontal[i];

        // Use 2D LBGK as base for weights
        s_weightsHorizontal[i] = weight * s_splitCoefficient;
        Assert(s_weightsHorizontal[i] > 0);
        Assert(s_weightsHorizontal[i] < 1);

        s_weightsVertical[i] = weight * 0.5 * (1.0 - s_splitCoefficient);
        Assert(s_weightsVertical[i] > 0);
        Assert(s_weightsVertical[i] < 1);
    }

}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
