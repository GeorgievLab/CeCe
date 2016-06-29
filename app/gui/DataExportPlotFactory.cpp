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
#include "DataExportPlotFactory.hpp"

// C++
#include <utility>

// CeCe
#include "DataExportPlot.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

UniquePtr<DataExport> DataExportPlotFactory::create(String name) const noexcept
{
    auto exporter = makeUnique<DataExportPlot>(std::move(name));

    // Register exporter
    m_exporters.push_back(exporter.get());

    return std::move(exporter);
}

/* ************************************************************************ */

ViewPtr<DataExportPlot> DataExportPlotFactory::getExporter(StringView name) const noexcept
{
    QString qname = name.getData();

    for (const auto& exporter : m_exporters)
    {
        if (exporter->getName() == qname)
            return exporter;
    }

    return nullptr;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
