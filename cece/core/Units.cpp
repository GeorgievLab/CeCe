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
#include "cece/core/Units.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

template class Unit<List<>, List<>>;
template class Unit<List<BaseLength>, List<>>;
template class Unit<List<BaseMass>, List<>>;
template class Unit<List<BaseTime>, List<>>;
template class Unit<List<BaseLength, BaseLength>, List<>>;
template class Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;
template class Unit<List<BaseLength>, List<BaseTime>>;
template class Unit<List<BaseLength>, List<BaseTime, BaseTime>>;
template class Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;
template class Unit<List<BaseLength, BaseMass>, List<BaseTime>>;
template class Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;
template class Unit<List<BaseMass>, List<BaseLength, BaseTime>>;
template class Unit<List<BaseLength, BaseLength>, List<BaseTime>>;
template class Unit<List<BaseAmountOfSubstance>, List<>>;
template class Unit<List<BaseAmountOfSubstance>, List<BaseLength, BaseLength, BaseLength>>;
template class Unit<List<>, List<BaseLength, BaseLength, BaseLength>>;
template class Unit<List<>, List<BaseTime>>;
template class Unit<List<BaseLength, BaseLength, BaseLength>, List<BaseTime>>;

/* ************************************************************************ */

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
