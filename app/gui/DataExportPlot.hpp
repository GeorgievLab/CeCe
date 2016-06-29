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

// Qt
#include <QObject>
#include <QString>

// CeCe
#include "cece/core/DataExport.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

class QCustomPlot;

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief Data exporting class - to GUI plot.
 */
class DataExportPlot : public QObject, public DataExport
{
    Q_OBJECT

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param name Exporter name.
     */
    explicit DataExportPlot(String name);


    /**
     * @brief Destructor.
     */
    ~DataExportPlot();


// Public Accessors
public:


    /**
     * @brief Returns exporter name.
     *
     * @return
     */
    const String& getName() const noexcept
    {
        return m_name;
    }


    /**
     * @brief Returns exported value names.
     *
     * @return
     */
    const DynamicArray<String>& getNames() const noexcept
    {
        return m_names;
    }


// Public Mutators
public:


    /**
     * @brief Set X axis column.
     * @param name
     */
    void setXColumn(StringView name) noexcept;


    /**
     * @brief Set Y axis column.
     * @param name
     */
    void setYColumn(StringView name) noexcept;


    /**
     * @brief Set color column.
     * @param name
     */
    void setColorColumn(StringView name) noexcept;


// Signals
signals:


    /**
     * @brief Data added.
     *
     * @param x     X value.
     * @param y     Y value.
     * @param color Color value.
     */
    void dataAdded(double x, double y, QString color);


// Protected Operations
protected:


    /**
     * @brief Write data header.
     *
     * @param count Number of columns.
     * @param ...   Column names.
     */
    void writeHeaderImpl(int count, ...) override;


    /**
     * @brief Write data record.
     *
     * @param count  Number of columns.
     * @param format Column format string.
     * @param ...    Column values.
     */
    void writeRecordImpl(int count, const char* format, ...) override;


// Private Data Members
private:

    /// Exporter name.
    String m_name;

    /// Exported value names.
    DynamicArray<String> m_names;

    /// X axis column index.
    int m_x = -1;

    /// Y axis column index.
    int m_y = -1;

    /// Color column index.
    int m_color = -1;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
