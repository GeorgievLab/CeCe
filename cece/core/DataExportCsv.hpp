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
#include "cece/core/DataExport.hpp"
#include "cece/core/CsvFile.hpp"
#include "cece/core/FilePath.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Data exporting class - to CSV file.
 */
class DataExportCsv : public DataExport
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param path
     */
    explicit DataExportCsv(FilePath path);


    /**
     * @brief Destructor.
     */
    ~DataExportCsv();


// Public Accessors
public:


    /**
     * @brief Returns path to output file.
     *
     * @return
     */
    const FilePath& getFilePath() const noexcept
    {
        return m_file.getPath();
    }


// Public Operations
public:


    /**
     * @brief Flush output.
     */
    void flush() override;


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
protected:

    /// CSV file.
    CsvFile m_file;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
