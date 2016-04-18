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

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/SharedPtr.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Map.hpp"
#include "cece/config/Implementation.hpp"

/* ************************************************************************ */

namespace cece {
namespace config {

/* ************************************************************************ */

/**
 * @brief Memory configuration implementation.
 */
class MemoryImplementation : public Implementation
{

// Private Structures
private:


    /**
     * @brief Data storage.
     */
    struct Data
    {
        /// Stored values.
        Map<String, String> values;

        /// Stored content.
        String content;

        /// Subdata
        Map<String, DynamicArray<SharedPtr<Data>>> data;
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    MemoryImplementation()
        : m_data(makeShared<Data>())
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param data Managed data.
     */
    explicit MemoryImplementation(SharedPtr<Data> data)
        : m_data(std::move(data))
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Check if value with given name exists.
     *
     * @param name Value name.
     *
     * @return
     */
    bool has(StringView name) const noexcept override
    {
        return m_data->values.find(name.getData()) != m_data->values.end();
    }


    /**
     * @brief Returns string value.
     *
     * @param name Value name.
     *
     * @return
     */
    String get(StringView name) const noexcept override
    {
        auto it = m_data->values.find(name.getData());
        return it != m_data->values.end() ? it->second : "";
    }


    /**
     * @brief Returns list of configuration names.
     *
     * @return
     */
    DynamicArray<String> getNames() const noexcept override;


    /**
     * @brief Returns if content string is set.
     *
     * @return
     */
    bool hasContent() const noexcept override
    {
        return !m_data->content.empty();
    }


    /**
     * @brief Returns content string.
     *
     * @return
     */
    String getContent() const noexcept override
    {
        return m_data->content;
    }


    /**
     * @brief Returns if sub-configuration exists.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    bool hasSubs(StringView name) const noexcept override
    {
        return m_data->data.find(name.getData()) != m_data->data.end();
    }


    /**
     * @brief Returns all sub-configuration with given name.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    DynamicArray<UniquePtr<Implementation>> getSubs(StringView name) const noexcept override;


    /**
     * @brief Returns list of sub-configuration names.
     *
     * @return
     */
    DynamicArray<String> getSubNames() const noexcept override;


// Public Mutators
public:


    /**
     * @brief Set string value.
     *
     * @param name  Value name.
     * @param value Value to store.
     *
     * @return
     */
    void set(StringView name, StringView value) noexcept override
    {
        m_data->values[name.getData()] = value.getData();
    }


    /**
     * @brief Store content.
     *
     * @param content Content text.
     */
    void setContent(StringView content) noexcept override
    {
        m_data->content = content.getData();
    }


    /**
     * @brief Create new sub-configuration.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    UniquePtr<Implementation> addSub(StringView name) noexcept override;


// Private Data Members
private:

    /// Shared data.
    SharedPtr<Data> m_data;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
