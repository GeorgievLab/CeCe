
#pragma once

/* ************************************************************************ */

// C++
#include <stdexcept>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

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

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
