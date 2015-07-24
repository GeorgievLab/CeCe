/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "core/Map.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Configuration.hpp"

/* ************************************************************************ */

namespace loader {
namespace reactions {

/* ************************************************************************ */

/**
 * @brief Reactions loader configuration implementation.
 */
class ConfigImplementation : public simulator::Configuration::Implementation
{

// Public Accessors
public:


    /**
     * @brief Check if value with given name exists.
     *
     * @param name Value name.
     *
     * @return
     */
    bool has(const StringView& name) const noexcept override
    {
        return m_values.find(name.getData()) != m_values.end();
    }


    /**
     * @brief Returns string value.
     *
     * @param name Value name.
     *
     * @return
     */
    String get(const StringView& name) const noexcept override
    {
        auto it = m_values.find(name.getData());
        return it != m_values.end() ? it->second : "";
    }


    /**
     * @brief Returns if content string is set.
     *
     * @return
     */
    bool hasContent() const noexcept override
    {
        return !m_content.empty();
    }


    /**
     * @brief Returns content string.
     *
     * @return
     */
    String getContent() const noexcept override
    {
        return m_content;
    }


    /**
     * @brief Returns if sub-configuration exists.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    bool hasSubs(const StringView& name) const noexcept override
    {
        return false;
    }


    /**
     * @brief Returns all sub-configuration with given name.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    DynamicArray<UniquePtr<Implementation>> getSubs(const StringView& name) const noexcept override
    {
        // TODO: sub configurations
        return {};
    }


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
    void set(const StringView& name, const StringView& value) noexcept override
    {
        m_values[name.getData()] = value.getData();
    }


    /**
     * @brief Store content.
     *
     * @param content Content text.
     */
    void setContent(const StringView& content) noexcept override
    {
        m_content = content.getData();
    }


// Private Data Members
private:


    /// Stored values.
    Map<String, String> m_values;

    /// Stored content.
    String m_content;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
