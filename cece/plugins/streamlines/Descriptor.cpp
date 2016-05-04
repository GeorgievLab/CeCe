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
#include "cece/plugins/streamlines/Descriptor.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

constexpr StaticArray<StaticArray<Descriptor::DirectionType, 3>, 3> Descriptor::INDEX_MAP;

/* ************************************************************************ */

const StaticArray<Descriptor::DirectionType, 3> Descriptor::TOP_LINE = INDEX_MAP[0];

/* ************************************************************************ */

const StaticArray<Descriptor::DirectionType, 3> Descriptor::MIDDLE_LINE = INDEX_MAP[1];

/* ************************************************************************ */

const StaticArray<Descriptor::DirectionType, 3> Descriptor::BOTTOM_LINE = INDEX_MAP[2];

/* ************************************************************************ */

const StaticArray<Descriptor::DirectionType, 3> Descriptor::LEFT_COLUMN = {{
    INDEX_MAP[0][0], INDEX_MAP[1][0], INDEX_MAP[2][0]
}};

/* ************************************************************************ */

const StaticArray<Descriptor::DirectionType, 3> Descriptor::MIDDLE_COLUMN = {{
    INDEX_MAP[0][1], INDEX_MAP[1][1], INDEX_MAP[2][1]
}};

/* ************************************************************************ */

const StaticArray<Descriptor::DirectionType, 3> Descriptor::RIGHT_COLUMN = {{
    INDEX_MAP[0][2], INDEX_MAP[1][2], INDEX_MAP[2][2]
}};

/* ************************************************************************ */

constexpr StaticArray<RealType, Descriptor::SIZE> Descriptor::DIRECTION_WEIGHTS;

/* ************************************************************************ */

constexpr StaticArray<Vector<int>, Descriptor::SIZE> Descriptor::DIRECTION_VELOCITIES;

/* ************************************************************************ */

constexpr StaticArray<Descriptor::DirectionType, Descriptor::SIZE> Descriptor::DIRECTION_OPPOSITES;

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
