
/* ************************************************************************ */

// Declaration
#include "LatticeData.hpp"

// C++
#include <cassert>

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

constexpr StaticArray<LatticeData::ValueType, LatticeData::SIZE> LatticeData::DIRECTION_WEIGHTS;

/* ************************************************************************ */

constexpr StaticArray<Vector<int>, LatticeData::SIZE> LatticeData::DIRECTION_VELOCITIES;

/* ************************************************************************ */

constexpr StaticArray<LatticeData::IndexType, LatticeData::SIZE> LatticeData::DIRECTION_OPPOSITES;

/* ************************************************************************ */

void LatticeData::collide(ValueType omega)
{
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
        const auto diff = calcVelocityNormalized() - m_dynamicObstacleVelocity;

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
            const auto feq = LatticeData::calcEquilibrium(calcVelocityNormalized(), calcRho());

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
        const auto feq = LatticeData::calcEquilibrium(calcVelocityNormalized(), calcRho());

        // Update values
        for (IndexType alpha = 0; alpha < SIZE; ++alpha)
        {
            m_values[alpha] += -omega * (m_values[alpha] - feq[alpha]);
        }
    }
}

/* ************************************************************************ */

StaticArray<LatticeData::ValueType, LatticeData::SIZE>
LatticeData::calcEquilibrium(const Vector<ValueType>& u, ValueType rho) NOEXCEPT
{
    static CONSTEXPR_CONST auto MAX_SPEED_SQ = MAX_SPEED * MAX_SPEED;
    auto uCopy = u;
    auto uLenSq = uCopy.getLengthSquared();

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
