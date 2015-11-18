/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include <algorithm>

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/Variant.hpp"
#include "cece/core/DynamicArray.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

/**
 * @brief Table for storing data.
 *
 * @tparam T Type of stored item.
 */
template<typename T>
class BasicDataTable
{

// Public Types
public:


    /// Value type.
    using ValueType = T;

    /// Column identifier.
    using ColumnId = unsigned int;

    /// Row identifier.
    using RowId = unsigned int;

    /// Row type.
    using RowType = DynamicArray<ValueType>;


// Public Constants
public:


    /// Invalid column ID constant.
    static constexpr ColumnId INVALID_COLUMN_ID = -1;


// Public Operators
public:


    /**
     * @brief Returns required row.
     *
     * @param id Row identifier.
     *
     * @return
     */
    RowType& operator[](RowId id) noexcept
    {
        return m_rows[id];
    }


    /**
     * @brief Returns required row.
     *
     * @param id Row identifier.
     *
     * @return
     */
    const RowType& operator[](RowId id) const noexcept
    {
        return m_rows[id];
    }


// Public Accessors
public:


    /**
     * @brief Returns column names.
     *
     * @return
     */
    const DynamicArray<String>& getColumns() const noexcept
    {
        return m_columns;
    }


    /**
     * @brief Returns rows.
     *
     * @return
     */
    const DynamicArray<RowType>& getRows() const noexcept
    {
        return m_rows;
    }


    /**
     * @brief Find column by name.
     *
     * @param name Column name.
     *
     * @return Id of given column or INVALID_COLUMN_ID.
     */
    ColumnId findColumn(const StringView& name) const noexcept
    {
        auto it = std::find(m_columns.begin(), m_columns.end(), name);
        return it != m_columns.end() ? it - m_columns.begin() : INVALID_COLUMN_ID;
    }


// Public Mutators
public:


    /**
     * @brief Add new column. In case the column name exists it's ID is returned.
     *
     * @param name Column name.
     *
     * @return Id of added column.
     */
    ColumnId addColumn(String name)
    {
        auto id = findColumn(name);

        // Already exists
        if (id != INVALID_COLUMN_ID)
            return id;

        m_columns.push_back(std::move(name));

        for (auto&& row : m_rows)
            row.emplace_back();

        return m_columns.size() - 1;
    }


    /**
     * @brief Add row.
     *
     * @return Id of added row.
     */
    RowId addRow()
    {
        m_rows.push_back(RowType(m_columns.size()));
        return m_rows.size() - 1;
    }


    /**
     * @brief Add row.
     *
     * @tparam Type  Type of the first value.
     * @tparam Types Types to store.
     *
     * @return Id of added row.
     */
    template<typename... Types>
    RowId addRow(Types&&... values)
    {
        auto id = addRow();
        setData(id, std::forward<Types>(values)...);
        return id;
    }


    /**
     * @brief Set row data.
     *
     * @tparam Value Value type.
     *
     * @param rowId Row id.
     */
    template<typename Key, typename Value>
    void setData(RowId rowId, Map<Key, Value>&& data)
    {
        auto& row = m_rows[rowId];

        for (const auto& item : data)
            row[addColumn(item.first)] = std::move(item.second);
    }


    /**
     * @brief Set row data.
     *
     * @tparam Value Value type.
     *
     * @param rowId Row id.
     */
    template<typename Key, typename Value>
    void setData(RowId rowId, const Map<Key, Value>& data)
    {
        auto& row = m_rows[rowId];

        for (const auto& item : data)
            row[addColumn(item.first)] = item.second;
    }


    /**
     * @brief Set row data.
     *
     * @tparam Value Value type.
     *
     * @param rowId Row id.
     * @param item  Item to store.
     */
    template<typename Key, typename Value>
    void setData(RowId rowId, Pair<Key, Value>&& item)
    {
        m_rows[rowId][addColumn(item.first)] = std::move(item.second);
    }


    /**
     * @brief Set row data.
     *
     * @tparam Value Value type.
     *
     * @param rowId Row id.
     * @param item  Item to store.
     */
    template<typename Key, typename Value>
    void setData(RowId rowId, const Pair<Key, Value>& item)
    {
        m_rows[rowId][addColumn(item.first)] = item.second;
    }


    /**
     * @brief Set row data.
     *
     * @tparam Type  First type.
     * @tparam Types Types to store.
     *
     * @param rowId Row id.
     */
    template<typename Type, typename... Types>
    void setData(RowId rowId, Type&& value, Types&&... values)
    {
        setData(rowId, std::forward<Type>(value));
        setData(rowId, std::forward<Types>(values)...);
    }


// Private Data Members
private:

    /// List of column names.
    DynamicArray<String> m_columns;

    /// Stored rows.
    DynamicArray<RowType> m_rows;

};

/* ************************************************************************ */

/**
 * @brief Default type for data table.
 */
using DataTableItem = Variant<
    int,
    unsigned int,
    long,
    unsigned long,
    long long,
    unsigned long long,
    RealType,
    String
>;

/* ************************************************************************ */

/**
 * @brief Default implementation of data table that can store anything.
 */
using DataTable = BasicDataTable<DataTableItem>;

/* ************************************************************************ */

/**
 * @brief Extern instantization.
 */
extern template class BasicDataTable<DataTableItem>;

/* ************************************************************************ */

}
}

/* ************************************************************************ */
