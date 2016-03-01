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

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/config/Implementation.hpp"

// pugixml
#include "pugixml/pugixml.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace xml {

/* ************************************************************************ */

/**
 * @brief XML configuration implementation.
 */
class ConfigImplementation : public config::Implementation
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
    bool has(StringView name) const noexcept override
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
    String get(StringView name) const noexcept override
    {
        return m_node.attribute(name.getData()).value();
    }


    /**
     * @brief Returns list of configuration names.
     *
     * @return
     */
    DynamicArray<String> getNames() const noexcept override
    {
        DynamicArray<String> names;

        for (auto attr : m_node.attributes())
            names.push_back(attr.name());

        return names;
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
    bool hasSubs(StringView name) const noexcept override
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
    DynamicArray<UniquePtr<config::Implementation>> getSubs(StringView name) const noexcept override
    {
        DynamicArray<UniquePtr<config::Implementation>> res;

        // Foreach children
        for (const auto& node : m_node.children(name.getData()))
            res.push_back(makeUnique<ConfigImplementation>(node.internal_object()));

        return res;
    }


    /**
     * @brief Returns list of sub-configuration names.
     *
     * @return
     */
    DynamicArray<String> getSubNames() const noexcept override
    {
        DynamicArray<String> names;

        for (auto child : m_node.children())
        {
            if (std::find(names.begin(), names.end(), child.name()) == names.end())
                names.push_back(child.name());
        }

        return names;
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
    void set(StringView name, StringView value) noexcept override
    {
        auto attr = m_node.attribute(name.getData());

        if (attr.empty())
            attr = m_node.append_attribute(name.getData());

        attr.set_value(value.getData());
    }


    /**
     * @brief Store content.
     *
     * @param content Content text.
     */
    void setContent(StringView content) noexcept override
    {
        m_node.text().set(content.getData());
    }


    /**
     * @brief Create new sub-configuration.
     *
     * @param name Sub-configuration name.
     *
     * @return
     */
    UniquePtr<config::Implementation> addSub(StringView name) noexcept override
    {
        return makeUnique<ConfigImplementation>(m_node.append_child(name.getData()));
    }


// Private Data Members
private:

    /// Managed node.
    pugi::xml_node m_node;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
