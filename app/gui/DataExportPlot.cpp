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
#include "DataExportPlot.hpp"

// C++
#include <cstdarg>
#include <utility>

// CeCe
#include "cece/core/StringView.hpp"
#include "cece/core/DynamicArray.hpp"

// GUI
#include "qcustomplot.h"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Find name.
 *
 * @param names
 * @param name
 *
 * @return
 */
int findName(const DynamicArray<String>& names, StringView name) noexcept
{
    for (unsigned int i = 0; i < names.size(); ++i)
    {
        if (names[i] == name)
            return i;
    }

    return -1;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

DataExportPlot::DataExportPlot(String name)
    : m_name(std::move(name))
{
    // Nothing to do
}

/* ************************************************************************ */

DataExportPlot::~DataExportPlot() = default;

/* ************************************************************************ */

void DataExportPlot::setXColumn(StringView name) noexcept
{
    m_x = findName(m_names, name);
}

/* ************************************************************************ */

void DataExportPlot::setYColumn(StringView name) noexcept
{
    m_y = findName(m_names, name);
}

/* ************************************************************************ */

void DataExportPlot::setColorColumn(StringView name) noexcept
{
    m_color = findName(m_names, name);
}

/* ************************************************************************ */

void DataExportPlot::writeHeaderImpl(int count, ...)
{
    va_list args;
    va_start(args, count);

    m_names.resize(count);

    for (int i = 0; i < count; ++i)
        m_names[i] = va_arg(args, const char*);

    va_end(args);
}

/* ************************************************************************ */

void DataExportPlot::writeRecordImpl(int count, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    double xValue = 0;
    double yValue = 0;
    QString colorValue;

    for (int i = 0; i < count; ++i)
    {
        switch (format[i])
        {
        case DATA_EXPORT_FORMAT_INT:
        {
            int value = va_arg(args, int);

            if (i == m_x)
                xValue = value;
            else if (i == m_y)
                yValue = value;
            else if (i == m_color)
                colorValue = QString::number(value);

            break;
        }

        case DATA_EXPORT_FORMAT_LONG:
        {
            long value = va_arg(args, long);

            if (i == m_x)
                xValue = value;
            else if (i == m_y)
                yValue = value;
            else if (i == m_color)
                colorValue = QString::number(value);

            break;
        }

        case DATA_EXPORT_FORMAT_DOUBLE:
        {
            double value = va_arg(args, double);

            if (i == m_x)
                xValue = value;
            else if (i == m_y)
                yValue = value;
            else if (i == m_color)
                colorValue = QString::number(value);

            break;
        }

        case DATA_EXPORT_FORMAT_STRING:
        {
            const char* value = va_arg(args, const char*);

            if (i == m_x)
                xValue = atof(value);
            else if (i == m_y)
                yValue = atof(value);
            else if (i == m_color)
                colorValue = QString(value);

            break;
        }
        }
    }

    va_end(args);

    emit dataAdded(xValue, yValue, colorValue);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
