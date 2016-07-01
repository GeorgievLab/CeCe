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
#include "cece/core/Log.hpp"

// C++
#include <iostream>

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

static Log::StreamOutput g_output(&std::cout);

/* ************************************************************************ */

static Log::StreamOutput g_error(&std::cerr);

/* ************************************************************************ */

Log::Output::~Output() = default;

/* ************************************************************************ */

Log::StreamOutput::~StreamOutput() = default;

/* ************************************************************************ */

void Log::StreamOutput::write(Type type, const String& section, const String& msg)
{
    if (!m_os)
        return;

    switch (type)
    {
    case Type::Default: break;
    case Type::Info:    *m_os << CliColor::LightGreen   << "[INFO] "  << CliColor::Default; break;
    case Type::Warning: *m_os << CliColor::Yellow       << "[WARN] "  << CliColor::Default; break;
    case Type::Error:   *m_os << CliColor::Red          << "[ERROR] " << CliColor::Default; break;
    case Type::Debug:   *m_os << CliColor::Green        << "[DEBUG] " << CliColor::Default; break;
    }

    if (!section.empty())
        *m_os << "[" << section << "] ";

    *m_os << msg << std::endl;
}

/* ************************************************************************ */

Log::Output* Log::s_output = &g_output;

/* ************************************************************************ */

Log::Output* Log::s_error = &g_error;

/* ************************************************************************ */

}
}

/* ************************************************************************ */
