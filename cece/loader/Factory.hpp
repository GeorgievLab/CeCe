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

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/Factory.hpp"

/* ************************************************************************ */

namespace cece {
namespace loader {

/* ************************************************************************ */

class Loader;

/* ************************************************************************ */

/**
 * @brief Simulation loader factory interface.
 */
using Factory = core::Factory<Loader>;

/* ************************************************************************ */

/**
 * @brief Loader factory for specific loader.
 *
 * @tparam LoaderType
 */
template<typename LoaderType>
using FactoryTyped = core::FactoryTyped<core::Factory, LoaderType, Loader>;

/* ************************************************************************ */

/**
 * @brief Loader factory with callable backend.
 *
 * @tparam Callable
 */
template<typename Callable>
using FactoryCallable = core::FactoryCallable<core::Factory, Callable, Loader>;

/* ************************************************************************ */

/**
 * @brief Make callable loader factory.
 *
 * @param callable Callable object.
 *
 * @return Callable loader factory.
 */
template<typename Callable>
FactoryCallable<Callable> makeCallableFactory(Callable callable) noexcept
{
    return FactoryCallable<Callable>{std::move(callable)};
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
