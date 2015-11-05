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
    m_dataFront.resize(size + 2);
    m_dataBack.resize(size + 2);
}

/* ************************************************************************ */

void Lattice::collide(LatticeData::ValueType omega)
{
    for (auto&& c : range(getSize()))
        get(c).collide(omega);
}

/* ************************************************************************ */

void Lattice::propagate()
{
    // Clear result grid
    for (auto& cell : m_dataBack)
        cell.clear();

    for (auto&& c : range(getSize()))
    {
        for (LatticeData::IndexType i = 0; i < LatticeData::SIZE; ++i)
        {
            // Calculate new coordinates
            Vector<LatticeData::IndexType> newCoord = c + LatticeData::DIRECTION_VELOCITIES[i];
            getBack(newCoord)[i] = getFront(c)[i];
        }

        getBack(c).setType(getFront(c).getType());
    }

    std::swap(m_dataFront, m_dataBack);
}

/* ************************************************************************ */

void Lattice::setType(LatticeData::Type type)
{
    for (auto& cell : m_dataFront)
        cell.setType(type);
}

/* ************************************************************************ */

void Lattice::fixupObstacles(LatticeData::Type type) noexcept
{
    using Offset = Vector<typename std::make_signed<LatticeData::IndexType>::type>;

    // Foreach all cells
    for (auto&& c : range(getSize()))
    {
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
