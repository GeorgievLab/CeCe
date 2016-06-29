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

// CeCe
#include "cece/core/ViewPtr.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/DataExportFactory.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

class DataExportPlot;

/* ************************************************************************ */

/**
 * @brief DataExportPlot factory.
 */
class DataExportPlotFactory : public DataExportFactory
{

// Public Accessors
public:


    /**
     * @brief Returns exporters.
     *
     * @return
     */
    const DynamicArray<ViewPtr<DataExportPlot>>& getExporters() const noexcept
    {
        return m_exporters;
    }


    /**
     * @brief Find exporter by name.
     *
     * @param name
     *
     * @return
     */
    ViewPtr<DataExportPlot> getExporter(StringView name) const noexcept;


// Public Operations
public:


    /**
     * @brief Create an object.
     *
     * @param name
     *
     * @return Created object pointer.
     */
    UniquePtr<DataExport> create(String name) const noexcept override;


    /**
     * @brief Reset factory.
     */
    void reset() noexcept
    {
        m_exporters.clear();
    }


// Private Data Members
private:

    /// Created exporters.
    mutable DynamicArray<ViewPtr<DataExportPlot>> m_exporters;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
