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

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

DataExportPlot::DataExportPlot(String name)
    : m_name(QString::fromStdString(name))
{
    // Nothing to do
}

/* ************************************************************************ */

DataExportPlot::~DataExportPlot() = default;

/* ************************************************************************ */

void DataExportPlot::writeHeaderImpl(int count, ...)
{
    va_list args;
    va_start(args, count);

    m_names.clear();

    for (int i = 0; i < count; ++i)
        m_names << QString(va_arg(args, const char*));

    va_end(args);
}

/* ************************************************************************ */

void DataExportPlot::writeRecordImpl(int count, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    QList<QVariant> values;

    for (int i = 0; i < count; ++i)
    {
        switch (format[i])
        {
        case DATA_EXPORT_FORMAT_INT:
            values << va_arg(args, int);
            break;

        case DATA_EXPORT_FORMAT_LONG:
            values << static_cast<long long>(va_arg(args, long));
            break;

        case DATA_EXPORT_FORMAT_DOUBLE:
            values << va_arg(args, double);
            break;

        case DATA_EXPORT_FORMAT_STRING:
            values << va_arg(args, const char*);
            break;
        }
    }

    va_end(args);

    // Send data
    emit dataAdded(m_names, values);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
