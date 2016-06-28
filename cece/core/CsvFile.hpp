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

// C++
#include <utility>

// CeCe
#include "cece/core/Tuple.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/String.hpp"
#include "cece/core/FilePath.hpp"
#include "cece/core/FileStream.hpp"
#include "cece/core/IntegerSequence.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief CSV file.
 */
class CsvFile
{

// Public Ctors & Dtors
public:


    /**
     * @brief Default constructor.
     */
    CsvFile() = default;


    /**
     * @brief Constructor.
     *
     * @param path Path to CSV file.
     */
    explicit CsvFile(FilePath path);


// Public Accessors
public:


    /**
     * @brief Returns path to output file.
     *
     * @return
     */
    const FilePath& getPath() const noexcept
    {
        return m_path;
    }


// Public Mutators
public:


    /**
     * @brief Set path to output file.
     *
     * @param filePath
     */
    void setPath(FilePath path) noexcept
    {
        m_path = std::move(path);
    }


// Public Operations
public:


    /**
     * @brief Open/reopen current CSV file.
     */
    void open();


    /**
     * @brief Open CSV file.
     *
     * @param path Path to CSV file.
     */
    void open(FilePath path);


    /**
     * @brief Close file.
     */
    void close() noexcept
    {
        m_file.close();
    }


    /**
     * @brief Write CSV file header.
     *
     * @param args
     */
    void writeHeaderArray(const DynamicArray<String>& values) noexcept
    {
        writeLineArray(values);
    }


    /**
     * @brief Write CSV file header.
     *
     * @param args
     */
    template<typename... Args>
    void writeHeader(Args&&... args) noexcept
    {
        writeLine(std::forward<Args>(args)...);
    }


    /**
     * @brief Write CSV record.
     *
     * @param args
     */
    void writeRecordArray(const DynamicArray<String>& values) noexcept
    {
        writeLineArray(values);
    }


    /**
     * @brief Write CSV record.
     *
     * @param args
     */
    template<typename... Args>
    void writeRecord(Args&&... args) noexcept
    {
        writeLine(std::forward<Args>(args)...);
    }


    /**
     * @brief Flush output.
     */
    void flush() noexcept
    {
        m_file.flush();
    }


// Protected Operations
protected:


    /**
     * @brief Write values.
     */
    void writeValues() noexcept
    {
        // Nothing
    }


    /**
     * @brief Write values.
     *
     * @param arg
     * @param args
     */
    template<typename Arg, typename... Args>
    void writeValues(Arg&& arg, Args&&... args) noexcept
    {
        m_file << ';' << arg;
        writeValues(std::forward<Args>(args)...);
    }


    /**
     * @brief Write CSV line.
     *
     * @param values
     */
    void writeLineArray(const DynamicArray<String>& values) noexcept
    {
        for (auto it = values.begin(); it != values.end(); ++it)
        {
            if (it != values.begin())
                m_file << ';';

            m_file << *it;
        }

        m_file << "\r\n";
    }


    /**
     * @brief Write CSV line.
     *
     * @param arg
     * @param args
     */
    template<typename Arg, typename... Args>
    void writeLine(Arg&& arg, Args&&... args) noexcept
    {
        m_file << arg;
        writeValues(std::forward<Args>(args)...);
        m_file << "\r\n";
    }


// Private Data Members
private:

    /// File path.
    FilePath m_path;

    /// File stream.
    FileStream m_file;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
