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
#include <memory>

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Unique smart pointer class.
 *
 * @tparam T       Type of managed object.
 * @tparam Deleter Deleter object type.
 */
template<typename T, class Deleter = std::default_delete<T>>
using UniquePtr = std::unique_ptr<T, Deleter>;

/* ************************************************************************ */

/**
 * @brief Make unique ptr function.
 *
 * @return
 */
template<typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args)
{
    return UniquePtr<T>(new T{std::forward<Args>(args)...});
}

/* ************************************************************************ */

/**
 * @brief Make unique ptr function for custom ctor & dtor.
 *
 * @param value Managed value.
 * @param dtor  Destructor function.
 *
 * @return
 */
template<typename T, typename Dtor>
auto makeUniqueCustom(T* value, Dtor dtor) -> UniquePtr<T, Dtor>
{
    return UniquePtr<T, Dtor>(value, dtor);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
