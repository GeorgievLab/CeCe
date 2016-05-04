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
#include "cece/core/Real.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/StaticArray.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Utility class which store LB configuration.
 */
class Descriptor
{

// Public Types
public:


    /// Population index type.
    using DirectionType = unsigned int;

    /// Density type.
    using DensityType = RealType;

    /// Momentum type.
    using MomentumType = Vector<RealType>;

    /// Velocity type.
    using VelocityType = Vector<RealType>;

    /// Direction index type. @obsolete
    using IndexType = DirectionType;


// Public Constants
public:

    /// Speed of sound squared.
    static constexpr RealType SPEED_OF_SOUND_SQ = 1.0 / 3.0;

    /// Speed of sound squared inversed.
    static constexpr RealType SPEED_OF_SOUND_SQ_INV = 1.0 / SPEED_OF_SOUND_SQ;

    /// Direction weight for center.
    static constexpr RealType WEIGHT_CENTER = 4.0 / 9.0;

    /// Direction weight for linear.
    static constexpr RealType WEIGHT_LINEAR = 1.0 / 9.0;

    /// Direction weight for diagonal.
    static constexpr RealType WEIGHT_DIAGONAL = 1.0 / 36.0;

    /// Number of populations.
    static constexpr DirectionType SIZE = 9;

    /// Default density.
    static constexpr DensityType DEFAULT_DENSITY = 1.0;

    /// Direction index map.
    static constexpr StaticArray<StaticArray<DirectionType, 3>, 3> INDEX_MAP{{
        {{1, 8, 7}},
        {{2, 0, 6}},
        {{3, 4, 5}}
    }};

    /// Direction index top line.
    static const StaticArray<DirectionType, 3> TOP_LINE;

    /// Direction index middle line.
    static const StaticArray<DirectionType, 3> MIDDLE_LINE;

    /// Direction index bottom line.
    static const StaticArray<DirectionType, 3> BOTTOM_LINE;

    /// Direction index left column.
    static const StaticArray<DirectionType, 3> LEFT_COLUMN;

    /// Direction index middle column.
    static const StaticArray<DirectionType, 3> MIDDLE_COLUMN;

    /// Direction index right column.
    static const StaticArray<DirectionType, 3> RIGHT_COLUMN;

    /// Direction weights.
    static constexpr StaticArray<RealType, SIZE> DIRECTION_WEIGHTS = {{
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

    /// Direction velocities.
    static constexpr StaticArray<Vector<int>, SIZE> DIRECTION_VELOCITIES = {{
        { 0,  0},
        {-1,  1}, {-1,  0}, {-1, -1}, { 0, -1}, { 1, -1}, { 1,  0}, { 1,  1}, { 0,  1}
    }};

    /// Direction opposites
    static constexpr StaticArray<DirectionType, SIZE> DIRECTION_OPPOSITES = {{
        0, 5, 6, 7, 8, 1, 2, 3, 4
    }};


// Public Types
public:


    /// Type of stored data.
    using DataType = StaticArray<DensityType, SIZE>;


// Public Operations
public:


    /**
     * @brief Calculare equilibrium function.
     *
     * @param weight   Direction weight (w_i).
     * @param dir      Direction vector (e_i).
     * @param density  Density (rho).
     * @param velocity Velocity (u).
     *
     * @return
     */
    template<typename WeightT, typename DirectionT, typename DensityT, typename VelocityT>
    static DensityT calcEquilibrium(WeightT weight, DirectionT dir, DensityT density, VelocityT velocity) noexcept
    {
        const auto vu = dot(dir, velocity);
        const auto uSq = velocity.getLengthSquared();

        return density * weight * (
              1.0
            + SPEED_OF_SOUND_SQ_INV * vu
            + 0.5 * SPEED_OF_SOUND_SQ_INV * SPEED_OF_SOUND_SQ_INV * vu * vu
            - 0.5 * SPEED_OF_SOUND_SQ_INV * uSq
        );
    }


    /**
     * @brief Calculate Poiseuille flow profile.
     *
     * @param max  Max speed.
     * @param size Range size.
     * @param pos  Position from range (0, size).
     *
     * @return
     */
    template<typename VelocityT, typename PositionT>
    static VelocityT calcPoiseuilleFlow(VelocityT max, PositionT size, PositionT pos) noexcept
    {
        Assert(pos >= 0);
        Assert(pos < size);
        Assert(size > 1);

        const auto posR = static_cast<RealType>(pos);
        const auto sizeR = static_cast<RealType>(size);
        return 4.0 * max / (sizeR * sizeR) * (sizeR * posR - posR * posR);
    }


    /**
     * @brief Returns an opposite index.
     *
     * @param iPop Population index.
     *
     * @return Opposite index of iPop.
     */
    static DirectionType opposite(DirectionType iPop) noexcept
    {
        return DIRECTION_OPPOSITES[iPop];
    }

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
