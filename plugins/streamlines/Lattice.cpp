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
#include "Lattice.hpp"

// Simulator
#include "core/VectorRange.hpp"

/* ************************************************************************ */

namespace plugin {
namespace streamlines {

/* ************************************************************************ */

void Lattice::setSize(Size size)
{
    // Data requires 1 cell margin
    //m_data.resize(size + 2);
    m_data.resize(size);
}

/* ************************************************************************ */

void Lattice::collide(LatticeData::ValueType omega)
{
    constexpr LatticeData::IndexType half = (LatticeData::SIZE - 1) / 2;

    for (auto&& c : range(getSize()))
    {
        auto& cell = get(c);
        cell.collide(omega);

        for (LatticeData::IndexType i = 1; i < half; ++i)
        {
            using std::swap;
            swap(cell[i], cell[i + half]);
        }
    }
}

/* ************************************************************************ */

void Lattice::stream()
{
    constexpr LatticeData::IndexType half = (LatticeData::SIZE - 1) / 2;

    for (auto&& c : range(getSize()))
    {
        for (LatticeData::IndexType i = 1; i < half; ++i)
        {
            // Calculate new coordinates
            const Vector<LatticeData::IndexType> newCoord = c + LatticeData::DIRECTION_VELOCITIES[i];

            // Swap
            if (inRange(newCoord))
            {
                using std::swap;
                swap(get(c)[i + half], get(newCoord)[i]);
            }
        }
    }
}

/* ************************************************************************ */

void Lattice::collideAndStream(LatticeData::ValueType omega)
{
    collide(omega);
    stream();
    /* FIXME: something wrong
    constexpr LatticeData::IndexType half = (LatticeData::SIZE - 1) / 2;

    for (auto&& c : range(getSize()))
    {
        auto& cell = get(c);
        cell.collide(omega);

        for (LatticeData::IndexType i = 1; i < half; ++i)
        {
            // Calculate new coordinates
            const Vector<LatticeData::IndexType> newCoord = c + LatticeData::DIRECTION_VELOCITIES[i];

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

void Lattice::setType(LatticeData::Type type)
{
    for (auto& cell : m_data)
        cell.setType(type);
}

/* ************************************************************************ */

void Lattice::fixupObstacles(LatticeData::Type type) noexcept
{
    using Offset = Vector<typename std::make_signed<LatticeData::IndexType>::type>;

    // Foreach all cells
    //for (auto&& c : range(Size{1, 1}, getSize() - Size{1, 1}))
    for (LatticeData::IndexType y = 1; y < getSize().getY() - 1; ++y)
    for (LatticeData::IndexType x = 1; x < getSize().getX() - 1; ++x)
    {
        CoordinateType c{x, y};

        if (get(c).getType() != type)
            continue;

        const LatticeData::Type types[9] = {
            get(c).getType(),
            get(c + Offset{ 1,  0}).getType(),
            get(c + Offset{-1,  0}).getType(),
            get(c + Offset{ 0,  1}).getType(),
            get(c + Offset{ 1,  1}).getType(),
            get(c + Offset{-1,  1}).getType(),
            get(c + Offset{ 0, -1}).getType(),
            get(c + Offset{ 1, -1}).getType(),
            get(c + Offset{-1, -1}).getType()
        };

        const bool test =
            (types[0] == type) &&
            (types[1] == type || types[1] == LatticeData::Type::None) &&
            (types[2] == type || types[2] == LatticeData::Type::None) &&
            (types[3] == type || types[3] == LatticeData::Type::None) &&
            (types[4] == type || types[4] == LatticeData::Type::None) &&
            (types[5] == type || types[5] == LatticeData::Type::None) &&
            (types[6] == type || types[6] == LatticeData::Type::None) &&
            (types[7] == type || types[7] == LatticeData::Type::None) &&
            (types[8] == type || types[8] == LatticeData::Type::None)
        ;

        if (test)
            get(c).setType(LatticeData::Type::None);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
