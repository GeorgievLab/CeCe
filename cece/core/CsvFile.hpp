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
#include "cece/export.hpp"
#include "cece/core/Tuple.hpp"
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

// Public Structures
public:


    /**
     * @brief Write condition.
     */
    template<typename... Args>
    struct Condition
    {
        bool expr;
        Tuple<Args...> values;
    };


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


// Public Mutators
public:


    /**
     * @brief Set printed value precision.
     *
     * @param value
     */
    void setPrecision(int value) noexcept
    {
        m_file.precision(value);
    }


// Public Operations
public:


    /**
     * @brief Create condition object.
     *
     * @param expr
     * @param args
     *
     * @return
     */
    template<typename... Args>
    static Condition<Args...> cond(bool expr, Args&&... args) noexcept
    {
        return Condition<Args...>{expr, Tuple<Args...>(std::forward<Args>(args)...)};
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
    template<typename... Args, int... Is>
    void writeTuple(const Tuple<Args...>& args, IntegerSequence<int, Is...>) noexcept
    {
        writeValues(getValue<Is>(args)...);
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
     * @param cond
     * @param args
     */
    template<typename... Args1, typename... Args2>
    void writeValues(Condition<Args1...> cond, Args2&&... args) noexcept
    {
        if (cond.expr)
            writeTuple(cond.values, MakeIntegerSequence<int, 0, sizeof...(Args1)>{});

        writeValues(std::forward<Args2>(args)...);
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

    /// File stream.
    FileStream m_file;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
