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
#include "LatticeCell.hpp"

// C++
#include <cstdlib>

// Simulator
#include "core/Assert.hpp"
#include "core/StaticArray.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

constexpr StaticArray<LatticeCell::IndexType, 3> TOP_LINE      {{1, 8, 7}};
constexpr StaticArray<LatticeCell::IndexType, 3> MIDDLE_LINE   {{2, 0, 6}};
constexpr StaticArray<LatticeCell::IndexType, 3> BOTTOM_LINE   {{3, 4, 5}};
constexpr StaticArray<LatticeCell::IndexType, 3> LEFT_COLUMN   {{1, 2, 3}};
constexpr StaticArray<LatticeCell::IndexType, 3> MIDDLE_COLUMN {{8, 0, 4}};
constexpr StaticArray<LatticeCell::IndexType, 3> RIGHT_COLUMN  {{7, 6, 5}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::DirCount> CENTER_RHO{{
    MIDDLE_COLUMN, // DirRight
    MIDDLE_COLUMN, // DirLeft
    MIDDLE_LINE,   // DirUp
    MIDDLE_LINE    // DirDown
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::DirCount> KNOWN_RHO{{
    LEFT_COLUMN,  // DirRight
    RIGHT_COLUMN, // DirLeft
    BOTTOM_LINE,  // DirUp
    TOP_LINE      // DirDown
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::DirCount> UNKNOWN_RHO{{
    RIGHT_COLUMN, // DirRight
    LEFT_COLUMN,  // DirLeft
    TOP_LINE,     // DirUp
    BOTTOM_LINE   // DirDown
}};

/* ************************************************************************ */

constexpr StaticArray<LatticeCell::VelocityType, LatticeCell::DirCount> VELOCITIES{{
    { 1,  0}, // DirRight
    {-1,  0}, // DirLeft
    { 0,  1}, // DirUp
    { 0, -1}  // DirDown
}};

/* ************************************************************************ */

constexpr StaticArray<LatticeCell::IndexType, LatticeCell::DirCount> BC_CENTER{{
    6, // DirRight
    2, // DirLeft
    8, // DirUp
    4  // DirDown
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::DirCount> BC_SIDE1{{
    {7, 4, 8}, // DirRight
    {1, 4, 8}, // DirLeft
    {7, 2, 6}, // DirUp
    {5, 2, 6}  // DirDown
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::DirCount> BC_SIDE2{{
    {5, 8, 4}, // DirRight
    {3, 8, 4}, // DirLeft
    {1, 6, 2}, // DirUp
    {3, 6, 2}  // DirDown
}};

/* ************************************************************************ */

}

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeCell::IndexType, 3>, 3> LatticeCell::INDEX_MAP;

/* ************************************************************************ */

constexpr StaticArray<LatticeCell::ValueType, LatticeCell::SIZE> LatticeCell::DIRECTION_WEIGHTS;

/* ************************************************************************ */

constexpr StaticArray<Vector<int>, LatticeCell::SIZE> LatticeCell::DIRECTION_VELOCITIES;

/* ************************************************************************ */

constexpr StaticArray<LatticeCell::IndexType, LatticeCell::SIZE> LatticeCell::DIRECTION_OPPOSITES;

/* ************************************************************************ */

void LatticeCell::inlet(const VelocityType& velocity, Direction dir) noexcept
{
    // Calculate inlet density from velocity
    const ValueType rho = fixVelocity(dir, velocity);
    Assert(rho > 0);

    // Initialize
    //init(v, rho);
    //return;

    fixBc(dir, velocity, rho);
}

/* ************************************************************************ */

void LatticeCell::outlet(Direction dir) noexcept
{
    constexpr ValueType rho = 1.0;

    // Velocity vector
    const auto vel = fixDensity(dir, rho);

    // Init
    init(vel, rho);
    return;
    //fixBc(dir, vel, rho);
    //return;

    switch (dir)
    {
    case Direction::DirRight:   fixBc(Direction::DirLeft, vel, rho); break;
    case Direction::DirLeft:    fixBc(Direction::DirRight, vel, rho); break;
    case Direction::DirUp:      fixBc(Direction::DirDown, vel, rho); break;
    case Direction::DirDown:    fixBc(Direction::DirUp, vel, rho); break;
    default: break;
    }
}

/* ************************************************************************ */

void LatticeCell::collide(ValueType omega) noexcept
{
    Assert(omega <= 1);

    switch (getType())
    {
    case Type::None:
        break;

    case Type::StaticObstacle:
        collideStatic(omega);
        break;

    case Type::DynamicObstacle:
        collideDynamic(omega);
        break;

    case Type::BGK:
        collideBgk(omega);
        break;
    }
}

/* ************************************************************************ */

void LatticeCell::collideStatic(ValueType omega) noexcept
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

/* ************************************************************************ */

void LatticeCell::collideDynamic(ValueType omega) noexcept
{
    // Velocity difference
    const auto diff = calcVelocity() - m_dynamicObstacleVelocity;

    if (diff > Zero)
    {
        // Slower -> obstacle
        collideStatic(omega);
    }
    else
    {
        // Faster -> No obstacle
        collideBgk(omega);
    }
}

/* ************************************************************************ */

void LatticeCell::collideBgk(ValueType omega) noexcept
{
    const auto feq = calcEquilibrium(calcVelocity(), calcRho());

    // Update values
    for (IndexType alpha = 0; alpha < SIZE; ++alpha)
    {
        m_values[alpha] += -omega * (m_values[alpha] - feq[alpha]);
        Assert(m_values[alpha] >= 0);
    }
}

/* ************************************************************************ */

LatticeCell::ValueType
LatticeCell::calcEquilibrium(IndexType i, const VelocityType& velocity, ValueType uSq, ValueType rho) noexcept
{
    const auto weight = DIRECTION_WEIGHTS[i];
    const auto vu = dot(DIRECTION_VELOCITIES[i], velocity);

    return rho * weight * (
          1.f
        + 3.f * vu
        + 9.f / 2.f * vu * vu
        - 3.f / 2.f * uSq
    );
}

/* ************************************************************************ */

StaticArray<LatticeCell::ValueType, LatticeCell::SIZE>
LatticeCell::calcEquilibrium(const VelocityType& velocity, ValueType rho) noexcept
{
    static constexpr auto MAX_SPEED_SQ = MAX_SPEED * MAX_SPEED;
    auto uCopy = velocity;
    auto uLenSq = uCopy.getLengthSquared();

    Assert(rho > 0);

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
        res[alpha] = calcEquilibrium(alpha, uCopy, uLenSq, rho);
    }

    return res;
}

/* ************************************************************************ */

LatticeCell::ValueType LatticeCell::fixVelocity(Direction dir, const VelocityType& velocity) const noexcept
{
    return
        RealType(1) / (RealType(1) - velocity.dot(VELOCITIES[dir]))
        * (2 * sumValues(KNOWN_RHO[dir]) + sumValues(CENTER_RHO[dir]))
    ;
}

/* ************************************************************************ */

LatticeCell::VelocityType LatticeCell::fixDensity(Direction dir, ValueType rho) const noexcept
{
    // Speed
    const auto speed =
        (-RealType(1) + RealType(1) / calcRho()
        * (sumValues(CENTER_RHO[dir]) + 2 * sumValues(UNKNOWN_RHO[dir]))
    );

    // Velocity vector
    return speed * VELOCITIES[dir];
}

/* ************************************************************************ */

void LatticeCell::fixBc(Direction dir, const VelocityType& velocity, ValueType rho) noexcept
{
    const auto center = BC_CENTER[dir];
    const auto side1 = BC_SIDE1[dir];
    const auto side2 = BC_SIDE2[dir];

    auto eq = calcEquilibrium(velocity, rho);

    m_values[center] = (eq[center] - eq[DIRECTION_OPPOSITES[center]]) + m_values[DIRECTION_OPPOSITES[center]];

    m_values[side1[0]] = (eq[side1[0]] - eq[DIRECTION_OPPOSITES[side1[0]]]) + m_values[DIRECTION_OPPOSITES[side1[0]]]
        + RealType(0.5) * (m_values[side1[1]] - m_values[side1[2]]);

    m_values[side2[0]] = (eq[side2[0]] - eq[DIRECTION_OPPOSITES[side2[0]]]) + m_values[DIRECTION_OPPOSITES[side2[0]]]
        + RealType(0.5) * (m_values[side2[1]] - m_values[side2[2]]);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
