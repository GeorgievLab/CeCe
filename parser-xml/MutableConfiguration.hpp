
#pragma once

/* ************************************************************************ */

// C++
#include <string>

// Simulator
#include "simulator/Configuration.hpp"

// pugixml
#include "pugixml/pugixml.hpp"

/* ************************************************************************ */

namespace parser {
namespace xml {

/* ************************************************************************ */

/**
 * @brief XML mutable configuration.
 */
class MutableConfiguration : public simulator::ConfigurationBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param node
     */
    explicit MutableConfiguration(pugi::xml_node& node) noexcept
        : m_node(node)
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns if value exists under given name.
     *
     * @param name
     *
     * @return
     */
    bool hasValue(const std::string& name) const override
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
    std::string getString(const std::string& name) const override
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
    int getInteger(const std::string& name) const override
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
    float getFloat(const std::string& name) const override
    {
        return m_node.attribute(name.c_str()).as_float();
    }


// Public Mutators
public:


    /**
     * @brief Set string value.
     *
     * @param name
     * @param value
     */
    void setString(const std::string& name, const std::string& value) override
    {
        m_node.attribute(name.c_str()).set_value(value.c_str());
    }


    /**
     * @brief Set integer value.
     *
     * @param name
     * @param value
     */
    void setInteger(const std::string& name, int value) override
    {
        m_node.attribute(name.c_str()).set_value(value);
    }


    /**
     * @brief Set float value.
     *
     * @param name
     * @param value
     */
    void setFloat(const std::string& name, float value) override
    {
        m_node.attribute(name.c_str()).set_value(value);
    }


// Private Data Members
private:


    /// Managed node.
    pugi::xml_node& m_node;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
