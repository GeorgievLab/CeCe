/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <boost/variant.hpp>

/* ************************************************************************ */

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

/* ************************************************************************ */
