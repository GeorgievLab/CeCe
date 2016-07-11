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
#include "cece/core/DataExportCsv.hpp"

// C++
#include <cstdarg>
#include <utility>

// CeCe
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

DataExportCsv::DataExportCsv(FilePath path)
    : m_file(path.replace_extension(".csv"))
{
    // Nothing to do
}

/* ************************************************************************ */

DataExportCsv::~DataExportCsv() = default;

/* ************************************************************************ */

void DataExportCsv::flush()
{
    m_file.flush();
}

/* ************************************************************************ */

void DataExportCsv::writeHeaderImpl(int count, ...)
{
    va_list args;
    va_start(args, count);

    DynamicArray<String> names(count);

    for (int i = 0; i < count; ++i)
        names[i] = va_arg(args, const char*);

    va_end(args);

    m_file.writeHeaderArray(names);
}

/* ************************************************************************ */

void DataExportCsv::writeRecordImpl(int count, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    DynamicArray<String> values(count);

    for (int i = 0; i < count; ++i)
    {
        switch (format[i])
        {
        case DATA_EXPORT_FORMAT_INT:
            values[i] = toString(va_arg(args, int));
            break;

        case DATA_EXPORT_FORMAT_LONG:
            values[i] = toString(va_arg(args, long));
            break;

        case DATA_EXPORT_FORMAT_DOUBLE:
            values[i] = toString(va_arg(args, double));
            break;

        case DATA_EXPORT_FORMAT_STRING:
            values[i] = va_arg(args, const char*);
            break;
        }
    }

    va_end(args);

    m_file.writeRecordArray(values);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
