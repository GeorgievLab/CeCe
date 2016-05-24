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
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace module {

/* ************************************************************************ */

void ExportModule::loadConfig(const config::Configuration& config)
{
    setFilePath(config.get<FilePath>("filename"));
}

/* ************************************************************************ */

void ExportModule::storeConfig(config::Configuration& config) const
{
    config.set("filename", getFilePath());
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

}
}

/* ************************************************************************ */
