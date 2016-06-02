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
#include "cece/module/ExportModule.hpp"

// CeCe
#include "cece/core/Log.hpp"
#include "cece/core/StringStream.hpp"
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace module {

/* ************************************************************************ */

bool ExportModule::isActive(IterationType it) const noexcept
{
    // No limitation
    if (m_active.empty())
        return true;

    for (const auto& rng : m_active)
    {
        if (rng.inRange(it))
            return true;
    }

    return false;
}

/* ************************************************************************ */

void ExportModule::loadConfig(const config::Configuration& config)
{
    setFilePath(config.get<FilePath>("filename"));
    setActive(parseActive(config.get("active", String{})));
}

/* ************************************************************************ */

void ExportModule::storeConfig(config::Configuration& config) const
{
    config.set("filename", getFilePath());
    // TODO: store active
}

/* ************************************************************************ */

void ExportModule::init()
{
    // Open CSV file
    m_file.open();

    Log::info("Exporting data into: ", getFilePath());
}

/* ************************************************************************ */

void ExportModule::terminate()
{
    Log::info("Data exported into: ", getFilePath());

    // Close CSV file
    m_file.close();
}

/* ************************************************************************ */

DynamicArray<IterationRange> ExportModule::parseActive(String str)
{
    DynamicArray<IterationRange> res;

    InStringStream iss(std::move(str));

    while (true)
    {
        IterationType it;

        if (!(iss >> it))
            break;

        if (iss.peek() == '-')
        {
            IterationType itEnd;
            iss.ignore();
            iss >> itEnd;

            res.emplace_back(it, itEnd);
        }
        else
        {
            // Single item range
            res.emplace_back(it);
        }
    }

    return res;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
