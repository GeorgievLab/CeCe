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
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/DynamicArray.hpp"
#include "core/Map.hpp"
#include "core/FilePath.hpp"
#include "simulator/Configuration.hpp"

// pugixml
#include "pugixml/pugixml.hpp"

/* ************************************************************************ */

namespace parser {
namespace xml {

/* ************************************************************************ */

/**
 * @brief XML immutable configuration.
 */
class DLL_EXPORT ImmutableConfiguration : public simulator::Configuration
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param node
     * @param filename
     */
    explicit ImmutableConfiguration(pugi::xml_node_struct* node, FilePath filename = {}) NOEXCEPT
        : m_node(node)
        , m_sourcePath(std::move(filename))
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param node
     * @param filename
     */
    explicit ImmutableConfiguration(const pugi::xml_node& node, FilePath filename = {}) NOEXCEPT
        : m_node(node.internal_object())
        , m_sourcePath(std::move(filename))
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns of the source file.
     *
     * @return
     */
    const FilePath& getSourcePath() const NOEXCEPT override
    {
        return m_sourcePath;
    }


    /**
     * @brief Returns if value exists under given name.
     *
     * @param name
     *
     * @return
     */
    bool hasValue(const String& name) const override
    {
        return !m_node.attribute(name.c_str()).empty();
    }


    /**
     * @brief Returns string value.
     *
     * @param name
     *
     * @return
     */
    String getString(const String& name) const override
    {
        return m_node.attribute(name.c_str()).value();
    }


    /**
     * @brief Returns integer value.
     *
     * @param name
     *
     * @return
     */
    int getInteger(const String& name) const override
    {
        return m_node.attribute(name.c_str()).as_int();
    }


    /**
     * @brief Returns float value.
     *
     * @param name
     *
     * @return
     */
    float getFloat(const String& name) const override
    {
        return m_node.attribute(name.c_str()).as_float();
    }


    /**
     * @brief Returns if there is content text.
     *
     * @return
     */
    bool hasText() const override
    {
        return !m_node.text().empty();
    }


    /**
     * @brief Returns text string.
     *
     * @return
     */
    String getText() const override
    {
        return m_node.text().get();
    }


    /**
     * @brief Returns configuration subconfiguration.
     *
     * @param name Configuration name.
     *
     * @return Configuration or nullptr.
     */
    virtual Configuration* getConfiguration(const String& name) const noexcept
    {
        auto list = getConfigurations(name);
        return list.empty() ? nullptr : list[0];
    }


    /**
     * @brief Returns configuration subconfigurations.
     *
     * @param name Configuration name.
     *
     * @return List of valid subconfigurations.
     */
    virtual DynamicArray<Configuration*> getConfigurations(const String& name) const noexcept
    {
        auto it = m_cache.find(name);

        if (it == m_cache.end())
        {
            DynamicArray<ImmutableConfiguration> list;

            for (const auto& node : m_node.children(name.c_str()))
            {
                list.emplace_back(node.internal_object(), m_sourcePath);
            }

            it = m_cache.emplace(name, std::move(list)).first;
        }

        DynamicArray<Configuration*> result;
        result.reserve(it->second.size());

        // Convert cache
        for (auto& config : it->second)
        {
            result.push_back(&config);
        }

        return result;
    }


// Public Mutators
public:


    /**
     * @brief Set string value.
     *
     * @param name
     * @param value
     */
    void setString(const String& name, const String& value) override
    {
        // Nothing to do
    }


    /**
     * @brief Set integer value.
     *
     * @param name
     * @param value
     */
    void setInteger(const String& name, int value) override
    {
        // Nothing to do
    }


    /**
     * @brief Set float value.
     *
     * @param name
     * @param value
     */
    void setFloat(const String& name, float value) override
    {
        // Nothing to do
    }


    /**
     * @brief Set text.
     *
     * @param text
     */
    void setText(String text) override
    {
        // Nothing to do
    }


// Private Data Members
private:

    /// Managed node.
    const pugi::xml_node m_node;

    /// Path to source file
    FilePath m_sourcePath;

    /// Subconfigurations cache.
    mutable Map<String, DynamicArray<ImmutableConfiguration>> m_cache;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
