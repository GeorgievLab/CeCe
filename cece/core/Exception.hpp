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
#include <stdexcept>
#include <typeinfo>

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Base exception class.
 */
using Exception = std::exception;

/* ************************************************************************ */

/**
 * @brief Logic exception class.
 */
using LogicException = std::logic_error;

/* ************************************************************************ */

/**
 * @brief Invalid argument exception class.
 */
using InvalidArgumentException = std::invalid_argument;

/* ************************************************************************ */

/**
 * @brief Domain exception class.
 */
using DomainException = std::domain_error;

/* ************************************************************************ */

/**
 * @brief Length exception class.
 */
using LengthException = std::length_error;

/* ************************************************************************ */

/**
 * @brief Out of range exception class.
 */
using OutOfRangeException = std::out_of_range;

/* ************************************************************************ */

/**
 * @brief Runtime exception class.
 */
using RuntimeException = std::runtime_error;

/* ************************************************************************ */

/**
 * @brief Range exception class.
 */
using RangeException = std::range_error;

/* ************************************************************************ */

/**
 * @brief Overflow exception class.
 */
using OverflowException = std::overflow_error;

/* ************************************************************************ */

/**
 * @brief Underflow exception class.
 */
using UndeflowException = std::underflow_error;

/* ************************************************************************ */

/**
 * @brief Bad cast exception.
 */
using BadCastException = std::bad_cast;

/* ************************************************************************ */

/**
 * @brief Invalid cast exception.
 */
struct InvalidCastException : public RuntimeException
{
    using RuntimeException::RuntimeException;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
