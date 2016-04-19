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
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/Exception.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/StringStream.hpp"
#include "cece/config/Exception.hpp"
#include "cece/config/Implementation.hpp"

/* ************************************************************************ */

namespace cece { inline namespace core { class Parameters; } }

/* ************************************************************************ */

namespace cece {
namespace config {

/* ************************************************************************ */

/**
 * @brief Container for configuration.
 */
class Configuration
{

// Public Ctors
public:


    /**
     * @brief Constructor.
     *
     * @param impl       Implementation.
     * @param parameters Optional parameters.
     */
    Configuration(UniquePtr<Implementation> impl, ViewPtr<Parameters> parameters = nullptr) noexcept
        : m_impl(std::move(impl))
        , m_parameters(parameters)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param impl       Implementation.
     * @param parameters Optional parameters.
     */
    Configuration(Implementation* impl, ViewPtr<Parameters> parameters = nullptr) noexcept
        : m_impl(impl)
        , m_parameters(parameters)
    {
        // Nothign to do
    }


    /**
     * @brief Constructor (memory version).
     *
     * @param parameters Optional parameters.
     */
    explicit Configuration(ViewPtr<Parameters> parameters = nullptr) noexcept;


// Public Accessors
public:


    /**
     * @brief Returns if value exists under given name.
     *
     * @param name Value name.
     *
     * @return
     */
    bool has(StringView name) const noexcept
    {
        return m_impl->has(name);
    }


    /**
     * @brief Returns string value.
     *
     * @param name Value name.
     *
     * @return
     *
     * @throw ConfigException
     */
    String get(StringView name) const
    {
        if (!has(name))
            throw config::Exception("Missing value for '" + String(name) + "'");

        return replaceParameters(m_impl->get(name));
    }


    /**
     * @brief Returns string value.
     *
     * @param name Value name.
     * @param def  Default value if doesn't exists.
     *
     * @return
     */
    String get(StringView name, String def) const noexcept
    {
        return has(name) ? replaceParameters(m_impl->get(name)) : std::move(def);
    }


    /**
     * @brief Returns value of given type.
     *
     * @tparam T Required value type.
     *
     * @param name Value name.
     *
     * @return
     *
     * @throw ConfigException
     */
    template<typename T>
    T get(StringView name) const
    {
        return castFrom<T>(get(name));
    }


    /**
     * @brief Returns value of given type.
     *
     * @tparam T Required value type.
     *
     * @param name Value name.
     * @param def  Default value if doesn't exists.
     *
     * @return
     */
    template<typename T>
    T get(StringView name, T def) const
    {
        return has(name) ? castFrom<T>(replaceParameters(m_impl->get(name))) : std::move(def);
    }


    /**
     * @brief Returns list of configuration names.
     *
     * @return
     */
    DynamicArray<String> getNames() const noexcept
    {
        return m_impl->getNames();
    }


    /**
     * @brief Returns if there is content.
     *
     * @return
     */
    bool hasContent() const noexcept
    {
        return m_impl->hasContent();
    }


    /**
     * @brief Returns content.
     *
     * @return
     */
    String getContent() const noexcept
    {
        return m_impl->getContent();
    }


    /**
     * @brief Check if subconfiguration exists.
     *
     * @param name Configuration name.
     *
     * @return Configuration or nullptr.
     */
    bool hasConfiguration(StringView name) const noexcept
    {
        return m_impl->hasSubs(name);
    }


    /**
     * @brief Returns the first sub-configuration.
     *
     * @param name Sub-configuration name.
     *
     * @return Sub-configuration.
     *
     * @throw ConfigException
     */
    Configuration getConfiguration(StringView name) const
    {
        auto configurations = getConfigurations(name);

        if (configurations.empty())
            throw config::Exception("Sub-configuration '" + String(name) + "' doesn't exists");

        return std::move(configurations[0]);
    }


    /**
     * @brief Returns all sub-configurations with given name.
     *
     * @param name Sub-configuration name.
     *
     * @return List of valid sub-configurations.
     */
    DynamicArray<Configuration> getConfigurations(StringView name) const noexcept;


    /**
     * @brief Returns list of sub-configuration names.
     *
     * @return
     */
    DynamicArray<String> getConfigurationNames() const noexcept
    {
        return m_impl->getSubNames();
    }


// Public Mutators
public:


    /**
     * @brief Store string value.
     *
     * @param name  Value name.
     * @param value Value to store.
     */
    void set(StringView name, StringView value) noexcept
    {
        m_impl->set(name, value);
    }


    /**
     * @brief Set integer value.
     *
     * @tparam T Value type.
     *
     * @param name  Value name.
     * @param value Value to store.
     */
    template<typename T>
    void set(StringView name, T value) noexcept
    {
        m_impl->set(name, castTo(value));
    }


    /**
     * @brief Set content text.
     *
     * @param content Content text.
     */
    void setContent(StringView content) noexcept
    {
        m_impl->setContent(content);
    }


    /**
     * @brief Create new sub-configuration.
     *
     * @param name Sub-configuration name.
     *
     * @return New configuration.
     */
    Configuration addConfiguration(StringView name) noexcept
    {
        return {m_impl->addSub(name)};
    }


// Public Operations
public:


    /**
     * @brief Copy configuration from other one.
     *
     * @param config Source configuration.
     */
    void copyFrom(const Configuration& config);


    /**
     * @brief Clone configuration to memory.
     *
     * @return
     */
    Configuration toMemory() const;


// Private Operations
private:


    /**
     * @brief Cast value from string into required type.
     *
     * @param value Value.
     *
     * @return
     */
    template<typename T>
    static T castFrom(const String& value)
    {
        InStringStream is(value);
        T res;
        is >> std::noskipws >> std::boolalpha >> res;
        return res;
    }


    /**
     * @brief Cast value to string from given type.
     *
     * @param value Value.
     *
     * @return
     */
    template<typename T>
    static String castTo(T&& value)
    {
        OutStringStream os;
        os << value;
        return os.str();
    }


    /**
     * @brief Replace parameters in given string.
     *
     * @param str Source string.
     *
     * @return Result string with replaced parameters.
     */
    String replaceParameters(String str) const;


// Private Data Members
private:

    /// Configuration implementation.
    UniquePtr<Implementation> m_impl;

    /// Optional parameters.
    ViewPtr<Parameters> m_parameters;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
