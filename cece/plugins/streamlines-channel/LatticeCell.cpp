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
#include <algorithm>

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

constexpr StaticArray<Vector<int>, LatticeCell::SIZE> LatticeCell::DIRECTION_VELOCITIES;

/* ************************************************************************ */

constexpr StaticArray<LatticeCell::IndexType, LatticeCell::SIZE> LatticeCell::DIRECTION_OPPOSITES;

/* ************************************************************************ */

StaticArray<LatticeCell::ValueType, LatticeCell::SIZE> LatticeCell::s_weights;

/* ************************************************************************ */

StaticArray<LatticeCell::ValueType, LatticeCell::SIZE> LatticeCell::s_weightsVertical;

/* ************************************************************************ */

void LatticeCell::initVelocity(Position position, const VelocityType& velocity, ValueType omega) noexcept
{
    // Calculate density from velocity
    const DensityType rho = calcVelocity(position, velocity, omega);
    Assert(rho > 0);

    // Apply boundary condition
    applyZouHe(position, velocity, rho);
}

/* ************************************************************************ */

void LatticeCell::initDensity(Position position, DensityType rho, ValueType omega) noexcept
{
    // Velocity vector
    const auto velocity = calcDensity(position, rho, omega);

    // Apply boundary condition
    applyZouHe(position, velocity, rho);
}

/* ************************************************************************ */

void LatticeCell::initInlet(Position position, const VelocityType& velocity, ValueType omega) noexcept
{
    initVelocity(position, velocity, omega);
}

/* ************************************************************************ */

void LatticeCell::initOutlet(Position position, DensityType rho, ValueType omega) noexcept
{
    initDensity(position, rho, omega);
}

/* ************************************************************************ */

LatticeCell::DensityType
LatticeCell::calcSum() const noexcept
{
    if (hasNoDynamics())
        return 0.0;

    ValueType res{};

    for (IndexType i = 0; i < SIZE; ++i)
        res += m_values[i];

    return res;
}

/* ************************************************************************ */

LatticeCell::DensityType
LatticeCell::calcRho() const noexcept
{
    const auto fSum = calcSum();
    const auto weights = std::accumulate(std::begin(s_weights), std::end(s_weights), ValueType(0.0));

    return fSum / weights;
}

/* ************************************************************************ */

LatticeCell::VelocityType
LatticeCell::calcMomentum() const noexcept
{
    if (hasNoDynamics())
        return Zero;

    VelocityType velocity = Zero;

    for (IndexType i = 0; i < SIZE; ++i)
        velocity += DIRECTION_VELOCITIES[i] * m_values[i];

    return velocity;
}

/* ************************************************************************ */

LatticeCell::VelocityType
LatticeCell::calcVelocity(ValueType omega) const noexcept
{
    ValueType cs2Wall = 0.0;

    // c_s^2 wall is same in all coordinate axes - calculate only for x.
    for (IndexType iPop = 0; iPop < SIZE; ++iPop)
        cs2Wall += s_weightsVertical[iPop] * DIRECTION_VELOCITIES[iPop][0] * DIRECTION_VELOCITIES[iPop][0];

    return calcMomentum() / (calcRho() * (1 - 2 * omega / (2 - omega) * cs2Wall * SPEED_OF_SOUND_SQ_INV));
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
    {
        temp[i] = m_values[DIRECTION_OPPOSITES[i]];
    }

    // Copy updated values
    m_values = temp;
#endif
}

/* ************************************************************************ */

void LatticeCell::collideDynamic(ValueType omega) noexcept
{
    // Velocity difference
    const auto diff = calcVelocity(omega) - m_dynamicObstacleVelocity;

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
    const auto rho = calcRho();
    const auto velocity = calcVelocity(omega);
    const auto feq = calcEquilibrium(velocity, rho);

    // Update values
    for (IndexType iPop = 0; iPop < SIZE; ++iPop)
    {
        // Collision
        m_values[iPop] += -omega * (m_values[iPop] - feq[iPop]);
        Assert(m_values[iPop] >= 0);
    }
}

/* ************************************************************************ */

void LatticeCell::stream(IndexType iPop, const LatticeCell& src)
{
    m_values[iPop] = src.m_values[iPop];
}

/* ************************************************************************ */

LatticeCell::DensityType
LatticeCell::calcVelocity(Position dir, const VelocityType& velocity, ValueType omega) const noexcept
{
    const auto center = sumValues(CENTER_RHO[dir]);
    const auto known  = sumValues(KNOWN_RHO[dir]);
    const auto velP = velocity.dot(VELOCITIES[dir]);

    ValueType wKWall = 0.0;

    for (auto iPop : KNOWN_RHO[dir])
        wKWall += s_weightsVertical[iPop];

    ValueType wWall = 0.0;

    for (IndexType iPop = 0; iPop < SIZE; ++iPop)
        wWall += s_weightsVertical[iPop];

    return
        (center + 2.0 * known)
        /
        (1.0 - velP * (1.0 + 4.0 * omega / (2.0 - omega) * SPEED_OF_SOUND_SQ_INV * wKWall) - 2.0 * wWall)
    ;
}

/* ************************************************************************ */

LatticeCell::VelocityType LatticeCell::calcDensity(Position dir, DensityType rho, ValueType omega) const noexcept
{
    const auto center = sumValues(CENTER_RHO[dir]);
    const auto known  = sumValues(KNOWN_RHO[dir]);

    ValueType wKWall = 0.0;

    for (auto iPop : KNOWN_RHO[dir])
        wKWall += s_weightsVertical[iPop];

    ValueType wWall = 0.0;

    for (IndexType iPop = 0; iPop < SIZE; ++iPop)
        wWall += s_weightsVertical[iPop];

    const auto speed =
        (1.0 - (1.0 / rho * (center + 2.0 * known) + 2.0 * wWall))
        /
        (1.0 + 4.0 * omega / (2.0 - omega) * SPEED_OF_SOUND_SQ_INV * wKWall)
    ;

    // Velocity vector
    return speed * VELOCITIES[dir];
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
LatticeCell::calcEquilibrium(ValueType weight, const Vector<int>& dirVel,
    const VelocityType& velocity, ValueType uSq, DensityType rho) noexcept
{
    const auto vu = dot(dirVel, velocity);

    return rho * weight * (
          1.0
        + SPEED_OF_SOUND_SQ_INV * vu
        + 0.5 * SPEED_OF_SOUND_SQ_INV * SPEED_OF_SOUND_SQ_INV * vu * vu
        - 0.5 * SPEED_OF_SOUND_SQ_INV * uSq
    );
}

/* ************************************************************************ */

StaticArray<LatticeCell::ValueType, LatticeCell::SIZE>
LatticeCell::calcEquilibrium(const VelocityType& velocity, DensityType rho) noexcept
{
    const auto uCopy = velocity;
    const auto uLenSq = uCopy.getLengthSquared();

    Assert(rho > 0);

    StaticArray<ValueType, SIZE> res;

    for (IndexType i = 0; i < SIZE; ++i)
        res[i] = calcEquilibrium(s_weights[i], DIRECTION_VELOCITIES[i], uCopy, uLenSq, rho);

    return res;
}

/* ************************************************************************ */

void LatticeCell::initModel(ValueType height)
{
    auto fHeight = [] (ValueType height) {
        return (1.0 / height);
        //return ((N - a) / (N - 1))
    };

    constexpr StaticArray<ValueType, 3> weights2d = {4.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0};
    StaticArray<ValueType, 6> weights;

    for (int i = 0; i < 3; ++i)
    {
        weights[i] = weights2d[i] / (1 + 0.5 * fHeight(height));
        weights[i + 3] = 0.25 * fHeight(height) * weights[i];
    }

    s_weights[0] = weights[0];
    s_weights[1] = weights[2];
    s_weights[2] = weights[1];
    s_weights[3] = weights[2];
    s_weights[4] = weights[1];
    s_weights[5] = weights[2];
    s_weights[6] = weights[1];
    s_weights[7] = weights[2];
    s_weights[8] = weights[1];

    s_weightsVertical[0] = weights[3];
    s_weightsVertical[1] = weights[5];
    s_weightsVertical[2] = weights[4];
    s_weightsVertical[3] = weights[5];
    s_weightsVertical[4] = weights[4];
    s_weightsVertical[5] = weights[5];
    s_weightsVertical[6] = weights[4];
    s_weightsVertical[7] = weights[5];
    s_weightsVertical[8] = weights[4];
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
