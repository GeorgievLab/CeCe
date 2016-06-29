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
#include <cstdarg>

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StaticArray.hpp"
#include "cece/core/Tuple.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/IntegerSequence.hpp"
#include "cece/core/StringStream.hpp"
#include "cece/core/DataExportFactory.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/// Format characters for basic types.
static constexpr char DATA_EXPORT_FORMAT_INT = 'i';
static constexpr char DATA_EXPORT_FORMAT_LONG = 'l';
static constexpr char DATA_EXPORT_FORMAT_DOUBLE = 'd';
static constexpr char DATA_EXPORT_FORMAT_STRING = 's';

/* ************************************************************************ */

/**
 * @brief Formatting structure.
 */
template<typename T>
struct DataExportFormat
{
    /// Format character.
    static constexpr char FORMAT = DATA_EXPORT_FORMAT_STRING;

    /// Types
    using StoreType = String;
    using PassType = const char*;


    /**
     * @brief Convert value.
     *
     * @param value Value to convert.
     *
     * @return Result string.
     */
    static StoreType convertStorage(T value)
    {
        OutStringStream oss;
        oss << value;

        return oss.str();
    }


    /**
     * @brief Convert value.
     *
     * @param value Value to convert.
     *
     * @return Result string.
     */
    static PassType convertPass(const StoreType& value)
    {
        return value.c_str();
    }
};

/* ************************************************************************ */

/**
 * @brief Formatting structure.
 */
template<typename Tin, typename Tout, char Format>
struct DataExportFormatBase
{
    /// Format character.
    static constexpr char FORMAT = Format;

    /// Types
    using StoreType = Tout;
    using PassType = Tout;


    /**
     * @brief Convert value.
     *
     * @param value Value to convert.
     *
     * @return Result string.
     */
    static StoreType convertStorage(Tin value) noexcept
    {
        return value;
    }


    /**
     * @brief Convert value.
     *
     * @param value Value to convert.
     *
     * @return Result string.
     */
    static PassType convertPass(StoreType value) noexcept
    {
        return value;
    }
};

/* ************************************************************************ */

template<>
struct DataExportFormat<int> : public DataExportFormatBase<int, int, DATA_EXPORT_FORMAT_INT> {};

template<>
struct DataExportFormat<unsigned int> : public DataExportFormatBase<unsigned int, int, DATA_EXPORT_FORMAT_INT> {};

template<>
struct DataExportFormat<long> : public DataExportFormatBase<long, long, DATA_EXPORT_FORMAT_LONG> {};

template<>
struct DataExportFormat<unsigned long> : public DataExportFormatBase<unsigned long, long, DATA_EXPORT_FORMAT_LONG> {};

template<>
struct DataExportFormat<long long> : public DataExportFormatBase<long long, long, DATA_EXPORT_FORMAT_LONG> {};

template<>
struct DataExportFormat<unsigned long long> : public DataExportFormatBase<unsigned long long, long, DATA_EXPORT_FORMAT_LONG> {};

template<>
struct DataExportFormat<double> : public DataExportFormatBase<double, double, DATA_EXPORT_FORMAT_DOUBLE> {};

template<>
struct DataExportFormat<float> : public DataExportFormatBase<float, double, DATA_EXPORT_FORMAT_DOUBLE> {};

/* ************************************************************************ */

/**
 * @brief Data exporting class.
 */
class DataExport
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~DataExport();


// Public Operations
public:


    /**
     * @brief Write CSV file header.
     *
     * @param args
     */
    template<typename... Args>
    void writeHeader(Args&&... args)
    {
        writeHeaderCall(
            MakeIntegerSequence<int, 0, sizeof...(Args)>{},
            std::forward<Args>(args)...
        );
    }


    /**
     * @brief Write CSV record.
     *
     * @param args
     */
    template<typename... Args>
    void writeRecord(Args&&... args)
    {
        writeRecordCall(
            MakeIntegerSequence<int, 0, sizeof...(Args)>{},
            std::forward<Args>(args)...
        );
    }


    /**
     * @brief Flush output.
     */
    virtual void flush();


    /**
     * @brief Set factory.
     *
     * @param factory A pointer to new factory.
     */
    static void setFactory(ViewPtr<DataExportFactory> factory) noexcept
    {
        s_factory = factory;
    }


    /**
     * @brief Create data export object.
     *
     * @param name
     *
     * @return
     */
    static UniquePtr<DataExport> create(String name);


// Protected Operations
protected:


    /**
     * @brief Write data header.
     *
     * @param count Number of columns.
     * @param ...   Column names.
     */
    virtual void writeHeaderImpl(int count, ...) = 0;


    /**
     * @brief Write data record.
     *
     * @param count  Number of columns.
     * @param format Column format string.
     * @param ...    Column values.
     */
    virtual void writeRecordImpl(int count, const char* format, ...) = 0;


// Private Operations
private:


    /**
     * @brief Write CSV file header.
     *
     * @param args
     */
    template<typename... Args, int... Is>
    void writeHeaderCall(IntegerSequence<int, Is...>, Args&&... args)
    {
        const StaticArray<String, sizeof...(args)> names{
            String(args)...
        };

        writeHeaderImpl(sizeof...(args), names[Is].c_str()...);
    }


    /**
     * @brief Write CSV record.
     *
     * @param args
     */
    template<typename... Args, int... Is>
    void writeRecordCall(IntegerSequence<int, Is...>, Args&&... args)
    {
        // Data format
        const char format[] = { DataExportFormat<typename std::decay<Args>::type>::FORMAT... };

        // Storage for arguments
        const Tuple<typename DataExportFormat<typename std::decay<Args>::type>::StoreType...> storage{
            DataExportFormat<typename std::decay<Args>::type>::convertStorage(std::forward<Args>(args))...
        };

        // Calling arguments
        const Tuple<typename DataExportFormat<typename std::decay<Args>::type>::PassType...> arguments{
            DataExportFormat<typename std::decay<Args>::type>::convertPass(getValue<Is>(storage))...
        };

        writeRecordImpl(
            sizeof...(args),
            format,
            getValue<Is>(arguments)...
        );
    }


// Private Data Members
private:

    /// Factory.
    static ViewPtr<DataExportFactory> s_factory;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
