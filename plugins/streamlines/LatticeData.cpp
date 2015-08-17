/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
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
    {0, 2, 4},
    {0, 2, 4},
    {0, 1, 3},
    {0, 1, 3}
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeData::IndexType, 3>, LatticeData::DirCount> NEXT_LINE{{
    {1, 5, 8},
    {3, 6, 7},
    {2, 5, 6},
    {4, 7, 8}
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeData::IndexType, 3>, LatticeData::DirCount> PREV_LINE{{
    {3, 6, 7},
    {1, 5, 8},
    {4, 7, 8},
    {2, 5, 6}
}};

/* ************************************************************************ */

constexpr StaticArray<Vector<LatticeData::ValueType>, LatticeData::DirCount> VELOCITIES{{
    { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}
}};

/* ************************************************************************ */

constexpr StaticArray<StaticArray<LatticeData::IndexType, 2>, LatticeData::DirCount> INDICES1{{
    {{3, 1}}, {{1, 3}}, {{2, 4}}, {{4, 2}}
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

    // Get velocity
    const auto vel = calcVelocity();
    const auto velDir = vel.dot(VELOCITIES[dir]);

    // Alias
    const auto& index1 = INDICES1[dir];

    // Microscopic boundary conditions: inlet (Zou/He BC)
    // fIn(2,in,col) = fIn(4,in,col) + 2/3*rho(:,in,col).*ux(:,in,col);
    m_values[index1[0]] = m_values[index1[1]]
        + RealType(2) / RealType(3) * rho * velDir;
    ;

    // fIn(6,in,col) = fIn(8,in,col) + 1/2*(fIn(5,in,col)-fIn(3,in,col)) ...
    //                               + 1/2*rho(:,in,col).*uy(:,in,col) ...
    //                               + 1/6*rho(:,in,col).*ux(:,in,col);
    m_values[5] = m_values[7]
        + RealType(1) / RealType(2) * (m_values[4] - m_values[2])
        + RealType(1) / RealType(2) * rho * vel.getY()
        + RealType(1) / RealType(6) * rho * vel.getX()
    ;

    // fIn(9,in,col) = fIn(7,in,col) + 1/2*(fIn(3,in,col)-fIn(5,in,col)) ...
    //                               - 1/2*rho(:,in,col).*uy(:,in,col) ...
    //                               + 1/6*rho(:,in,col).*ux(:,in,col);
    m_values[8] = m_values[6]
        + RealType(1) / RealType(2) * (m_values[2] - m_values[4])
        - RealType(1) / RealType(2) * rho * vel.getY()
        + RealType(1) / RealType(6) * rho * vel.getX()
    ;
}

/* ************************************************************************ */

void LatticeData::outlet(Direction dir) noexcept
{
    constexpr ValueType rho = 1.0;

    // Speed
    const auto speed = (-RealType(1) + RealType(1) / rho * (sumValues(CENTER_LINE[dir]) + 2 * sumValues(NEXT_LINE[dir])));

    // Velocity vector
    const Vector<ValueType> vel = speed * VELOCITIES[dir];

    // Init
    init(vel, rho);

    // Alias
    const auto& index1 = INDICES1[dir];

    // fIn(4,out,col) = fIn(2,out,col) - 2/3*rho(:,out,col).*ux(:,out,col);
    m_values[index1[0]] = m_values[index1[1]] - RealType(2) / RealType(3) * rho * speed;

    // fIn(8,out,col) = fIn(6,out,col) + 1/2*(fIn(3,out,col)-fIn(5,out,col)) ...
    //                                 - 1/2*rho(:,out,col).*uy(:,out,col) ...
    //                                 - 1/6*rho(:,out,col).*ux(:,out,col);
    m_values[7] = m_values[5]
        + RealType(1) / RealType(2) * (m_values[2] - m_values[4])
        - RealType(1) / RealType(2) * rho * vel.getY()
        + RealType(1) / RealType(6) * rho * vel.getX()
    ;

    // fIn(7,out,col) = fIn(9,out,col) + 1/2*(fIn(5,out,col)-fIn(3,out,col)) ...
    //                                 + 1/2*rho(:,out,col).*uy(:,out,col) ...
    //                                 - 1/6*rho(:,out,col).*ux(:,out,col);
    m_values[6] = m_values[8]
        + RealType(1) / RealType(2) * (m_values[4] - m_values[2])
        + RealType(1) / RealType(2) * rho * vel.getY()
        - RealType(1) / RealType(6) * rho * vel.getX()
    ;
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
        // FIXME: Remove Vector construction.
        uCopy *= Vector<ValueType>(MAX_SPEED_SQ / uLenSq);
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
