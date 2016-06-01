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

#pragma once

/* ************************************************************************ */

// C++
#include <cmath>
#include <cstdint>

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Fast version of exp function.
 *
 * @param arg
 *
 * @return
 *
 * @see A Fast, Compact Approximation of the Exponential Function
 */
inline double fastexp(double arg) noexcept
{
    union {
        double d;
        struct {
// FIXME: only GCC (and Clang)?
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            int32_t j, i;
#else
            int32_t i, j;
#endif
        } n;
    } eco;

    // TODO: replace log(2) by constant
    const int32_t EXP_A = 1048576 / log(2);
    constexpr int32_t EXP_C = 60801;

    eco.n.i = EXP_A * arg + (1072693248 - EXP_C);

    return eco.d;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
