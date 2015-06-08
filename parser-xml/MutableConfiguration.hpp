
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/FilePath.hpp"
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
class DLL_EXPORT MutableConfiguration : public simulator::ConfigurationBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param node
     * @param filename
     */
    explicit MutableConfiguration(pugi::xml_node& node, FilePath filename = {}) NOEXCEPT
        : m_node(node)
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
    const String& getSourcePath() const NOEXCEPT override
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
        m_node.attribute(name.c_str()).set_value(value.c_str());
    }


    /**
     * @brief Set integer value.
     *
     * @param name
     * @param value
     */
    void setInteger(const String& name, int value) override
    {
        m_node.attribute(name.c_str()).set_value(value);
    }


    /**
     * @brief Set float value.
     *
     * @param name
     * @param value
     */
    void setFloat(const String& name, float value) override
    {
        m_node.attribute(name.c_str()).set_value(value);
    }


    /**
     * @brief Set text.
     *
     * @param text
     */
    void setText(String text) override
    {
        m_node.text().set(text.c_str());
    }


// Private Data Members
private:

    /// Managed node.
    pugi::xml_node& m_node;

    /// Path to source file
    FilePath m_sourcePath;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
