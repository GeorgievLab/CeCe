
#pragma once

/* ************************************************************************ */

// C++
#include <vector>
#include <string>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

namespace core {

/* ************************************************************************ */

/// Application arguments
extern std::vector<std::string> args;

/* ************************************************************************ */

/**
 * @brief Initialize ARGS
 *
 * @param argc
 * @param argv
 */
inline void init_args(int argc, char** argv) NOEXCEPT
{
    args.assign(argv, argv + argc);
}

/* ************************************************************************ */

}

/* ************************************************************************ */
