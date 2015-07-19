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
#include "core/DynamicArray.hpp"
#include "simulator/Configuration.hpp"

// pugixml
#include "pugixml/pugixml.hpp"

/* ************************************************************************ */

namespace loader {
namespace xml {

/* ************************************************************************ */

/**
 * @brief XML configuration implementation.
 */
class ConfigImplementation : public simulator::Configuration::Implementation
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param node     XML node.
     * @param filename Path to source file.
     */
    ConfigImplementation(pugi::xml_node_struct* node) noexcept
        : m_node(node)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param node     XML node.
     * @param filename Path to source file.
     */
    ConfigImplementation(const pugi::xml_node& node) noexcept
        : m_node(node.internal_object())
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
    bool has(const StringView& name) const noexcept override
    {
        return !m_node.attribute(name.getData()).empty();
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
        return m_node.attribute(name.getData()).value();
    }


    /**
     * @brief Returns if content string is set.
     *
     * @return
     */
    bool hasContent() const noexcept override
    {
        return !m_node.text().empty();
    }


    /**
     * @brief Returns content string.
     *
     * @return
     */
    String getContent() const noexcept override
    {
        return m_node.text().get();
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
        auto rng = m_node.children(name.getData());
        return rng.begin() != rng.end();
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
        DynamicArray<UniquePtr<Implementation>> res;

        // Foreach children
        for (const auto& node : m_node.children(name.getData()))
            res.push_back(makeUnique<ConfigImplementation>{node.internal_object()});

        return res;
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
        m_node.attribute(name.getData()).set_value(value.getData());
    }


    /**
     * @brief Store content.
     *
     * @param value Content.
     */
    void setContent(const StringView& value) noexcept override
    {
        m_node.text().set(text.getData());
    }


// Private Data Members
private:

    /// Managed node.
    const pugi::xml_node m_node;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
