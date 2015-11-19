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
#include "cece/plugins/streamlines/LatticeCell.hpp"

// C++
#include <utility>

// CeCe
#include "cece/core/Assert.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

const StaticArray<LatticeCell::IndexType, 3> TOP_LINE      = LatticeCell::INDEX_MAP[0];

/* ************************************************************************ */

const StaticArray<LatticeCell::IndexType, 3> MIDDLE_LINE   = LatticeCell::INDEX_MAP[1];

/* ************************************************************************ */

const StaticArray<LatticeCell::IndexType, 3> BOTTOM_LINE   = LatticeCell::INDEX_MAP[2];

/* ************************************************************************ */

const StaticArray<LatticeCell::IndexType, 3> LEFT_COLUMN   = {{
    LatticeCell::INDEX_MAP[0][0],
    LatticeCell::INDEX_MAP[1][0],
    LatticeCell::INDEX_MAP[2][0]
}};

/* ************************************************************************ */

const StaticArray<LatticeCell::IndexType, 3> MIDDLE_COLUMN {{
    LatticeCell::INDEX_MAP[0][1],
    LatticeCell::INDEX_MAP[1][1],
    LatticeCell::INDEX_MAP[2][1]
}};

/* ************************************************************************ */

const StaticArray<LatticeCell::IndexType, 3> RIGHT_COLUMN  {{
    LatticeCell::INDEX_MAP[0][2],
    LatticeCell::INDEX_MAP[1][2],
    LatticeCell::INDEX_MAP[2][2]
}};

/* ************************************************************************ */

const StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::PositionCount> CENTER_RHO{{
    MIDDLE_COLUMN, // PositionRight
    MIDDLE_COLUMN, // PositionLeft
    MIDDLE_LINE,   // PositionTop
    MIDDLE_LINE    // PositionBottom
}};

/* ************************************************************************ */

const StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::PositionCount> KNOWN_RHO{{
    LEFT_COLUMN,  // PositionRight
    RIGHT_COLUMN, // PositionLeft
    BOTTOM_LINE,  // PositionTop
    TOP_LINE      // PositionBottom
}};

/* ************************************************************************ */

const StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::PositionCount> UNKNOWN_RHO{{
    RIGHT_COLUMN, // PositionRight
    LEFT_COLUMN,  // PositionLeft
    TOP_LINE,     // PositionTop
    BOTTOM_LINE   // PositionBottom
}};

/* ************************************************************************ */

const StaticArray<LatticeCell::VelocityType, LatticeCell::PositionCount> VELOCITIES{{
    { 1,  0}, // PositionRight
    {-1,  0}, // PositionLeft
    { 0,  1}, // PositionTop
    { 0, -1}  // PositionBottom
}};

/* ************************************************************************ */

const StaticArray<LatticeCell::IndexType, LatticeCell::PositionCount> BC_CENTER{{
    LatticeCell::INDEX_MAP[1][2], // PositionRight
    LatticeCell::INDEX_MAP[1][0], // PositionLeft
    LatticeCell::INDEX_MAP[0][1], // PositionTop
    LatticeCell::INDEX_MAP[2][1]  // PositionBottom
}};

/* ************************************************************************ */

const StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::PositionCount> BC_SIDE1{{
    {{LatticeCell::INDEX_MAP[0][2], LatticeCell::INDEX_MAP[2][1], LatticeCell::INDEX_MAP[0][1]}}, // PositionRight
    {{LatticeCell::INDEX_MAP[0][0], LatticeCell::INDEX_MAP[2][1], LatticeCell::INDEX_MAP[0][1]}}, // PositionLeft
    {{LatticeCell::INDEX_MAP[0][2], LatticeCell::INDEX_MAP[1][0], LatticeCell::INDEX_MAP[1][2]}}, // PositionTop
    {{LatticeCell::INDEX_MAP[2][2], LatticeCell::INDEX_MAP[1][0], LatticeCell::INDEX_MAP[1][2]}}  // PositionBottom
}};

/* ************************************************************************ */

const StaticArray<StaticArray<LatticeCell::IndexType, 3>, LatticeCell::PositionCount> BC_SIDE2{{
    {{LatticeCell::INDEX_MAP[2][2], LatticeCell::INDEX_MAP[0][1], LatticeCell::INDEX_MAP[2][1]}}, // PositionRight
    {{LatticeCell::INDEX_MAP[2][0], LatticeCell::INDEX_MAP[0][1], LatticeCell::INDEX_MAP[2][1]}}, // PositionLeft
    {{LatticeCell::INDEX_MAP[0][0], LatticeCell::INDEX_MAP[1][2], LatticeCell::INDEX_MAP[1][0]}}, // PositionTop
    {{LatticeCell::INDEX_MAP[2][0], LatticeCell::INDEX_MAP[1][2], LatticeCell::INDEX_MAP[1][0]}}  // PositionBottom
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

void LatticeCell::initVelocity(Position position, const VelocityType& velocity) noexcept
{
    // Calculate density from velocity
    const DensityType rho = calcVelocity(position, velocity);
    Assert(rho > 0);

    // Apply boundary condition
    applyZouHe(position, velocity, rho);
}

/* ************************************************************************ */

void LatticeCell::initDensity(Position position, DensityType rho) noexcept
{
    // Velocity vector
    const auto velocity = calcDensity(position, rho);

    // Apply boundary condition
    applyZouHe(position, velocity, rho);
}

/* ************************************************************************ */

void LatticeCell::initInlet(Position position, const VelocityType& velocity) noexcept
{
    initVelocity(position, velocity);
}

/* ************************************************************************ */

void LatticeCell::initOutlet(Position position, DensityType rho) noexcept
{
    initDensity(position, rho);
}

/* ************************************************************************ */

LatticeCell::VelocityType LatticeCell::calcVelocity() const noexcept
{
    if (hasNoDynamics())
        return Zero;

    return VelocityType{
        sumValues(RIGHT_COLUMN) - sumValues(LEFT_COLUMN),
        sumValues(TOP_LINE) - sumValues(BOTTOM_LINE)
    };
}

/* ************************************************************************ */

void LatticeCell::collide(ValueType omega) noexcept
{
    switch (getDynamics())
    {
    case Dynamics::None:
        break;

    case Dynamics::StaticObstacle:
        collideStatic(omega);
        break;

    case Dynamics::DynamicObstacle:
        collideDynamic(omega);
        break;

    case Dynamics::BGK:
        collideBgk(omega);
        break;
    }
}

/* ************************************************************************ */

void LatticeCell::collideStatic(ValueType omega) noexcept
{
#if DEV_PLUGIN_streamlines_SWAP_TRICK
    constexpr LatticeCell::IndexType half = (LatticeCell::SIZE - 1) / 2;

    for (IndexType i = 1; i <= half; ++i)
    {
        using std::swap;
        swap(m_values[i], m_values[i + half]);
    }
#else
    // Static obstacle, bounce all back
    StaticArray<ValueType, SIZE> temp;

    // Move updated values into opposite directions
    for (IndexType i = 0; i < SIZE; ++i)
        temp[i] = m_values[DIRECTION_OPPOSITES[i]];

    // Copy updated values
    m_values = temp;
#endif
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

LatticeCell::DensityType LatticeCell::calcVelocity(Position dir, const VelocityType& velocity) const noexcept
{
    return
        RealType(1) / (RealType(1) - velocity.dot(VELOCITIES[dir]))
        * (2 * sumValues(KNOWN_RHO[dir]) + sumValues(CENTER_RHO[dir]))
    ;
}

/* ************************************************************************ */

LatticeCell::VelocityType LatticeCell::calcDensity(Position dir, DensityType rho) const noexcept
{
    // Speed
    const auto speed =
        (-RealType(1) + RealType(1) / rho
        * (sumValues(CENTER_RHO[dir]) + 2 * sumValues(KNOWN_RHO[dir]))
    );

    // Velocity vector
    return ValueType(-1.0) * speed * VELOCITIES[dir];
}

/* ************************************************************************ */

void LatticeCell::applyZouHe(Position dir, const VelocityType& velocity, DensityType rho) noexcept
{
    const auto center = BC_CENTER[dir];
    const auto side1 = BC_SIDE1[dir];
    const auto side2 = BC_SIDE2[dir];

    const auto eq = calcEquilibrium(velocity, rho);

    auto eqDiff = [&eq] (IndexType i) {
        return eq[i] - eq[DIRECTION_OPPOSITES[i]];
    };

    m_values[center] = m_values[DIRECTION_OPPOSITES[center]] + eqDiff(center);

    m_values[side1[0]] = m_values[DIRECTION_OPPOSITES[side1[0]]] + eqDiff(side1[0])
        + RealType(0.5) * (m_values[side1[1]] - m_values[side1[2]])
        - RealType(0.5) * eqDiff(side1[1]);

    m_values[side2[0]] = m_values[DIRECTION_OPPOSITES[side2[0]]] + eqDiff(side2[0])
        + RealType(0.5) * (m_values[side2[1]] - m_values[side2[2]])
        - RealType(0.5) * eqDiff(side2[1]);
}

/* ************************************************************************ */

LatticeCell::ValueType
LatticeCell::calcEquilibrium(IndexType i, const VelocityType& velocity, ValueType uSq, DensityType rho) noexcept
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
LatticeCell::calcEquilibrium(const VelocityType& velocity, DensityType rho) noexcept
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

    for (IndexType i = 0; i < SIZE; ++i)
        res[i] = calcEquilibrium(i, uCopy, uLenSq, rho);

    return res;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
