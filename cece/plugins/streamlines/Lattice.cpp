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
#include "cece/plugins/streamlines/Lattice.hpp"

// C++
#include <utility>

// CeCe
#include "cece/core/VectorRange.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

void Lattice::collide(LatticeCell::ValueType omega)
{
#if DEV_PLUGIN_streamlines_SWAP_TRICK
    constexpr LatticeCell::IndexType half = (LatticeCell::SIZE - 1) / 2;

    for (auto&& c : range(getSize()))
    {
        auto& cell = get(c);
        cell.collide(omega);

        for (LatticeCell::IndexType i = 1; i < half; ++i)
        {
            using std::swap;
            swap(cell[i], cell[i + half]);
        }
    }
#else
    for (auto&& c : range(getSize()))
        get(c).collide(omega);
#endif
}

/* ************************************************************************ */

void Lattice::stream()
{
#if DEV_PLUGIN_streamlines_SWAP_TRICK
    constexpr LatticeCell::IndexType half = (LatticeCell::SIZE - 1) / 2;

    for (auto&& c : range(getSize()))
    {
        for (LatticeCell::IndexType i = 1; i < half; ++i)
        {
            // Calculate new coordinates
            const Vector<LatticeCell::IndexType> newCoord = c + LatticeCell::DIRECTION_VELOCITIES[i];

            // Swap
            if (inRange(newCoord))
            {
                using std::swap;
                swap(get(c)[i + half], get(newCoord)[i]);
            }
        }
    }
#else
    for (auto&& c : range(getSize()))
    {
        for (LatticeCell::IndexType i = 0; i < LatticeCell::SIZE; ++i)
        {
            // Calculate new coordinates
            Vector<LatticeCell::IndexType> newCoord = c + LatticeCell::DIRECTION_VELOCITIES[i];

            if (inRange(newCoord))
            {
                getBack(newCoord)[i] = get(c)[i];
            }
        }

        getBack(c).setDynamics(get(c).getDynamics());
    }

    std::swap(m_data, m_dataBack);
#endif
}

/* ************************************************************************ */

void Lattice::collideAndStream(LatticeCell::ValueType omega)
{
    collide(omega);
    stream();
    /*
    // FIXME: something's wrong
    constexpr LatticeCell::IndexType half = (LatticeCell::SIZE - 1) / 2;
    const auto size = getSize();

    //for (auto&& c : range(getSize()))
    for (CoordinateType::ValueType x = 0; x < size.getWidth(); ++x)
    for (CoordinateType::ValueType y = 0; y < size.getHeight(); ++y)
    {
        const CoordinateType c{x, y};

        auto& cell = get(c);
        cell.collide(omega);

        for (LatticeCell::IndexType i = 1; i < half; ++i)
        {
            // Calculate new coordinates
            const Vector<LatticeCell::IndexType> newCoord = c + LatticeCell::DIRECTION_VELOCITIES[i];

            if (inRange(newCoord))
            {
                auto& cellNext = get(newCoord);

                const auto tmp = cell[i];
                cell[i] = cell[i + half];
                cell[i + half] = cellNext[i];
                cellNext[i] = tmp;
            }
        }
    }
    */
}

/* ************************************************************************ */

void Lattice::setDynamics(LatticeCell::Dynamics dynamics)
{
    for (auto& cell : m_data)
        cell.setDynamics(dynamics);
}

/* ************************************************************************ */

void Lattice::fixupObstacles(LatticeCell::Dynamics dynamics) noexcept
{
    using Offset = Vector<typename std::make_signed<LatticeCell::IndexType>::type>;

    // Foreach all cells
    //for (auto&& c : range(Size{1, 1}, getSize() - Size{1, 1}))
    for (LatticeCell::IndexType y = 1; y < getSize().getY() - 1; ++y)
    for (LatticeCell::IndexType x = 1; x < getSize().getX() - 1; ++x)
    {
        CoordinateType c{x, y};

        if (get(c).getDynamics() != dynamics)
            continue;

        const LatticeCell::Dynamics types[9] = {
            get(c).getDynamics(),
            get(c + Offset{ 1,  0}).getDynamics(),
            get(c + Offset{-1,  0}).getDynamics(),
            get(c + Offset{ 0,  1}).getDynamics(),
            get(c + Offset{ 1,  1}).getDynamics(),
            get(c + Offset{-1,  1}).getDynamics(),
            get(c + Offset{ 0, -1}).getDynamics(),
            get(c + Offset{ 1, -1}).getDynamics(),
            get(c + Offset{-1, -1}).getDynamics()
        };

        const bool test =
            (types[0] == dynamics) &&
            (types[1] == dynamics || types[1] == LatticeCell::Dynamics::None) &&
            (types[2] == dynamics || types[2] == LatticeCell::Dynamics::None) &&
            (types[3] == dynamics || types[3] == LatticeCell::Dynamics::None) &&
            (types[4] == dynamics || types[4] == LatticeCell::Dynamics::None) &&
            (types[5] == dynamics || types[5] == LatticeCell::Dynamics::None) &&
            (types[6] == dynamics || types[6] == LatticeCell::Dynamics::None) &&
            (types[7] == dynamics || types[7] == LatticeCell::Dynamics::None) &&
            (types[8] == dynamics || types[8] == LatticeCell::Dynamics::None)
        ;

        if (test)
            get(c).setDynamics(LatticeCell::Dynamics::None);
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
