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

// Declaration
#include "LatticeData.hpp"

// C++
#include <cassert>
#include <cstdlib>

// Simulator
#include "core/StaticArray.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeData::IndexType, 3>, LatticeData::DirCount> CENTER_LINE{{
    {{0, 2, 4}},
    {{0, 2, 4}},
    {{0, 1, 3}},
    {{0, 1, 3}}
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeData::IndexType, 3>, LatticeData::DirCount> NEXT_LINE{{
    {{1, 5, 8}},
    {{3, 6, 7}},
    {{2, 5, 6}},
    {{4, 7, 8}}
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeData::IndexType, 3>, LatticeData::DirCount> PREV_LINE{{
    {{3, 6, 7}},
    {{1, 5, 8}},
    {{4, 7, 8}},
    {{2, 5, 6}}
}};

/* ************************************************************************ */

constexpr StaticArray<Vector<LatticeData::ValueType>, LatticeData::DirCount> VELOCITIES{{
    { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}
}};

/* ************************************************************************ */

}

/* ************************************************************************ */

constexpr StaticArray<LatticeData::ValueType, LatticeData::SIZE> LatticeData::DIRECTION_WEIGHTS;

/* ************************************************************************ */

constexpr StaticArray<Vector<int>, LatticeData::SIZE> LatticeData::DIRECTION_VELOCITIES;

/* ************************************************************************ */

constexpr StaticArray<LatticeData::IndexType, LatticeData::SIZE> LatticeData::DIRECTION_OPPOSITES;

/* ************************************************************************ */

void LatticeData::inlet(const Vector<ValueType>& v, Direction dir) noexcept
{
    // rho(:,in,col) = 1 ./ (1-ux(:,in,col)) .* ( ...
    // sum(fIn([1,3,5],in,col)) + 2*sum(fIn([4,7,8],in,col)) );
    const ValueType rho =
        RealType(1)
        / (RealType(1) - v.dot(VELOCITIES[dir]))
        * (sumValues(CENTER_LINE[dir]) + 2 * sumValues(PREV_LINE[dir]));

    assert(rho > 0);

    // Initialize
    init(v, rho);

    // Zou/He
    //microscopicBc(dir, calcVelocity(), rho);
}

/* ************************************************************************ */

void LatticeData::outlet(Direction dir) noexcept
{
    constexpr ValueType rho = 1.0;

    // Speed
    const auto speed =
        (-RealType(1) + RealType(1) / calcRho()
        * (sumValues(CENTER_LINE[dir]) + 2 * sumValues(NEXT_LINE[dir]))
    );

    // Velocity vector
    const Vector<ValueType> vel = speed * VELOCITIES[dir];

    // Init
    init(vel, rho);

    // Zou/He
    //microscopicBc(dir, vel, rho);
}

/* ************************************************************************ */

void LatticeData::collide(ValueType omega)
{
    assert(omega <= 1);

    if (isStaticObstacle())
    {
        // Static obstacle, bounce all back
        StaticArray<ValueType, SIZE> temp;

        // Move updated values into opposite directions
        for (IndexType i = 0; i < SIZE; ++i)
        {
            temp[i] = m_values[DIRECTION_OPPOSITES[i]];
        }

        // Copy updated values
        m_values = temp;
    }
    else if (isDynamicObstacle())
    {
        // Velocity difference
        const auto diff = calcVelocity() - m_dynamicObstacleVelocity;

        if (diff > Zero)
        {
            // Partial obstacle
            // Static obstacle, bounce all back
            StaticArray<ValueType, SIZE> temp;

            // Move updated values into opposite directions
            for (IndexType i = 0; i < SIZE; ++i)
            {
                temp[i] = m_values[DIRECTION_OPPOSITES[i]];
            }

            // Copy updated values
            m_values = temp;
        }
        else
        {
            // No obstacle

            // Calculate
            const auto feq = calcEquilibrium(calcVelocity(), calcRho());

            // Update values
            for (IndexType alpha = 0; alpha < SIZE; ++alpha)
            {
                m_values[alpha] += -omega * (m_values[alpha] - feq[alpha]);
            }
        }
    }
    else
    {
        // Calculate
        const auto feq = calcEquilibrium(calcVelocity(), calcRho());

        // Update values
        for (IndexType alpha = 0; alpha < SIZE; ++alpha)
        {
            m_values[alpha] += -omega * (m_values[alpha] - feq[alpha]);
            assert(m_values[alpha] >= 0);
        }
    }
}

/* ************************************************************************ */

void LatticeData::microscopicBc(Direction dir, const Vector<ValueType>& vel, ValueType rho) noexcept
{
    // FIXME: broken
#if BROKEN
    constexpr StaticArray<StaticArray<IndexType, 2>, DirCount> INDICES1{{
        {{1, 3}}, {{3, 1}}, {{2, 4}}, {{4, 2}}
    }};
    constexpr StaticArray<StaticArray<IndexType, 4>, DirCount> INDICES2{{
        {{5, 7, 4, 2}}, {{7, 5, 2, 4}}, {{5, 7, 3, 1}}, {{7, 5, 1, 3}}
    }};
    constexpr StaticArray<StaticArray<IndexType, 4>, DirCount> INDICES3{{
        {{8, 6, 2, 4}}, {{6, 8, 4, 2}}, {{6, 8, 1, 3}}, {{8, 6, 3, 1}}
    }};
    constexpr StaticArray<Vector<ValueType>, DirCount> VELOCITIES_ORT{{
        { 0,  1}, { 0, -1}, { 1,  0}, {-1,  0}
    }};
    constexpr StaticArray<LatticeData::ValueType, DirCount> MULTIPLICATOR_ORT{{
        -1, 1, -1, 1
    }};

    const auto velDir = vel.dot(VELOCITIES[dir]);
    const auto velOrt = vel.dot(VELOCITIES_ORT[dir]);

    // Alias
    const auto& index1 = INDICES1[dir];
    const auto& index2 = INDICES2[dir];
    const auto& index3 = INDICES3[dir];

    // First fix
    m_values[index1[0]] = m_values[index1[1]] - RealType(2) / RealType(3) * rho * velDir;

    // Second fix
    m_values[index2[0]] = m_values[index2[1]]
        + RealType(1) / RealType(2) * (m_values[index2[2]] - m_values[index2[3]])
        + RealType(1) / RealType(2) * rho * velOrt
        + RealType(1) / RealType(6) * rho * velDir
    ;

    // Third fix
    m_values[index3[0]] = m_values[index3[1]]
        + RealType(1) / RealType(2) * (m_values[index3[2]] - m_values[index3[3]])
        + RealType(1) / RealType(2) * rho * MULTIPLICATOR_ORT[dir] * velOrt
        + RealType(1) / RealType(6) * rho * velDir
    ;
#endif
}

/* ************************************************************************ */

StaticArray<LatticeData::ValueType, LatticeData::SIZE>
LatticeData::calcEquilibrium(const Vector<ValueType>& u, ValueType rho) noexcept
{
    static constexpr auto MAX_SPEED_SQ = MAX_SPEED * MAX_SPEED;
    auto uCopy = u;
    auto uLenSq = uCopy.getLengthSquared();

    assert(rho > 0);

    // Check if velocity is in range
    if (uLenSq > MAX_SPEED_SQ)
    {
        // Limit velocity
        uCopy *= MAX_SPEED_SQ / uLenSq;
        uLenSq = MAX_SPEED_SQ;
    }

    StaticArray<ValueType, SIZE> res;

    for (IndexType alpha = 0; alpha < SIZE; ++alpha)
    {
        const auto weight = DIRECTION_WEIGHTS[alpha];
        const auto velocity = DIRECTION_VELOCITIES[alpha];
        const auto vu = dot(velocity, uCopy);

        res[alpha] = rho * weight * (
              1.f
            + 3.f * vu
            + 9.f / 2.f * vu * vu
            - 3.f / 2.f * uLenSq
        );
        assert(res[alpha] >= 0);
    }

    return res;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
