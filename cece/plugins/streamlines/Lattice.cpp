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

// Declaration
#include "cece/plugins/streamlines/Lattice.hpp"

// C++
#include <cstddef>
#include <utility>

// CeCe
#include "cece/core/VectorRange.hpp"

// Plugin
#include "cece/plugins/streamlines/Descriptor.hpp"
#include "cece/plugins/streamlines/NoDynamics.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

void Lattice::initEquilibrium()
{
    for (auto& node : m_data)
        node.initEquilibrium();
}

/* ************************************************************************ */

void Lattice::collide()
{
#ifdef CECE_PLUGIN_streamlines_SWAP_TRICK
    constexpr Descriptor::DirectionType half = (Descriptor::SIZE - 1) / 2;

    for (auto&& c : range(getSize()))
    {
        auto& cell = get(c);
        cell.collide();

        // TODO: block swap
        for (Descriptor::DirectionType i = 1; i <= half; ++i)
        {
            using std::swap;
            swap(cell[i], cell[i + half]);
        }
    }
#else
    for (auto&& c : range(getSize()))
        get(c).collide();
#endif
}

/* ************************************************************************ */

void Lattice::stream()
{
#ifdef CECE_PLUGIN_streamlines_SWAP_TRICK
    constexpr Descriptor::DirectionType half = (Descriptor::SIZE - 1) / 2;

    for (auto&& c : range(getSize()))
    {
        for (Descriptor::DirectionType i = 1; i <= half; ++i)
        {
            // Calculate new coordinates
            const Vector<Descriptor::DirectionType> newCoord = c + Descriptor::DIRECTION_VELOCITIES[i];

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
        for (Descriptor::IndexType i = 0; i < Descriptor::SIZE; ++i)
        {
            // Calculate new coordinates
            Vector<Descriptor::IndexType> newCoord = c + Descriptor::DIRECTION_VELOCITIES[i];

            if (inRange(newCoord))
            {
                Assert(get(c)[i] > 0);
                getBack(newCoord)[i] = get(c)[i];
            }
        }

        getBack(c).setDynamics(get(c).getDynamics());
    }

    std::swap(m_data, m_dataBack);
#endif
}

/* ************************************************************************ */

void Lattice::collideAndStream()
{
    collide();
    stream();
/*
    // FIXME: something's wrong
    constexpr Descriptor::IndexType half = (Descriptor::SIZE - 1) / 2;
    const auto size = getSize();

    //for (auto&& c : range(getSize()))
    for (CoordinateType::ValueType x = 0; x < size.getWidth(); ++x)
    for (CoordinateType::ValueType y = 0; y < size.getHeight(); ++y)
    {
        const CoordinateType c{x, y};

        auto& cell = get(c);
        cell.collide();

        for (Descriptor::IndexType i = 1; i <= half; ++i)
        {
            // Calculate new coordinates
            const Vector<Descriptor::IndexType> newCoord = c + Descriptor::DIRECTION_VELOCITIES[i];

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

void Lattice::setDynamics(ViewPtr<Dynamics> dynamics)
{
    for (auto& cell : m_data)
        cell.setDynamics(dynamics);
}

/* ************************************************************************ */

void Lattice::fixupObstacles(ViewPtr<Dynamics> dynamics) noexcept
{
    using Offset = Vector<typename std::make_signed<Descriptor::IndexType>::type>;

    static constexpr StaticArray<Offset, 9> OFFSETS{{
        Offset{ 0,  0},
        Offset{ 1,  0}, Offset{-1,  0}, Offset{ 0,  1}, Offset{ 1,  1},
        Offset{-1,  1}, Offset{ 0, -1}, Offset{ 1, -1}, Offset{-1, -1}
    }};

    auto noDynamics = NoDynamics::getInstance();

    // Foreach all cells
    for (auto&& c : range(getSize()))
    {
        if (get(c).getDynamics() != dynamics)
            continue;

        bool test = true;

        for (std::size_t i = 0; i < OFFSETS.size(); ++i)
        {
            ViewPtr<Dynamics> type;

            if (inRange(c + OFFSETS[i]))
                type = get(c + OFFSETS[i]).getDynamics();

            test = test && (type == nullptr || type == dynamics || type == noDynamics);
        }

        if (test)
            get(c).setDynamics(noDynamics);
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
