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
#include <boost/variant.hpp>

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Type for storing one of given type.
 */
template<typename... Types>
using Variant = boost::variant<Types...>;

/* ************************************************************************ */

/**
 * @brief Variant static visitor.
 */
template<typename ResultType>
using VariantStaticVisitor = boost::static_visitor<ResultType>;

/* ************************************************************************ */

using boost::get;

/* ************************************************************************ */

/**
 * @brief Apply visitor.
 *
 * @param args...
 *
 * @return
 */
template<typename... Args>
auto variantApplyVisitor(Args&&... args) -> decltype(boost::apply_visitor(std::forward<Args>(args)...))
{
    return boost::apply_visitor(std::forward<Args>(args)...);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
