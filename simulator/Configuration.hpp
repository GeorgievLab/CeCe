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
#include "core/FilePath.hpp"
#include "core/UniquePtr.hpp"
#include "core/Exception.hpp"
#include "core/DynamicArray.hpp"
#include "core/StringStream.hpp"
//#include "core/LexicalCast.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Configuration exception.
 */
class ConfigException : public RuntimeException
{
    using RuntimeException::RuntimeException;
};

/* ************************************************************************ */

/**
 * @brief Simulation configuration.
 */
class Configuration
{

// Public Types
public:


    /**
     * @brief Configuration implementation.
     */
    class Implementation
    {

    // Public Ctors & Dtors
    public:


        /**
         * @brief Destructor.
         */
        virtual ~Implementation()
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
        virtual bool has(const StringView& name) const noexcept = 0;


        /**
         * @brief Returns list of configuration names.
         *
         * @return
         */
        virtual DynamicArray<String> getNames() const noexcept = 0;


        /**
         * @brief Returns string value.
         *
         * @param name Value name.
         *
         * @return
         */
        virtual String get(const StringView& name) const noexcept = 0;


        /**
         * @brief Returns if content string is set.
         *
         * @return
         */
        virtual bool hasContent() const noexcept = 0;


        /**
         * @brief Returns content string.
         *
         * @return
         */
        virtual String getContent() const noexcept = 0;


        /**
         * @brief Returns if sub-configuration exists.
         *
         * @param name Sub-configuration name.
         *
         * @return
         */
        virtual bool hasSubs(const StringView& name) const noexcept = 0;


        /**
         * @brief Returns all sub-configuration with given name.
         *
         * @param name Sub-configuration name.
         *
         * @return
         */
        virtual DynamicArray<UniquePtr<Implementation>> getSubs(const StringView& name) const noexcept = 0;


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
        virtual void set(const StringView& name, const StringView& value) noexcept = 0;


        /**
         * @brief Store content.
         *
         * @param value Content.
         */
        virtual void setContent(const StringView& value) noexcept = 0;


        /**
         * @brief Create new sub-configuration.
         *
         * @param name Sub-configuration name.
         *
         * @return
         */
        virtual UniquePtr<Implementation> addSub(const StringView& name) noexcept = 0;


        /**
         * @brief Returns list of sub-configuration names.
         *
         * @return
         */
        virtual DynamicArray<String> getSubNames() const noexcept = 0;

    };


// Public Ctors
public:


    /**
     * @brief Default constructor.
     */
    Configuration() = default;


    /**
     * @brief Constructor.
     *
     * @param impl Implementation.
     * @param path Path to source file.
     */
    Configuration(UniquePtr<Implementation> impl, FilePath path) noexcept
        : m_impl(std::move(impl))
        , m_filePath(std::move(path))
    {
        // Nothign to do
    }


    /**
     * @brief Constructor.
     *
     * @param impl Implementation.
     * @param path Path to source file.
     */
    Configuration(Implementation* impl, FilePath path) noexcept
        : m_impl(impl)
        , m_filePath(std::move(path))
    {
        // Nothign to do
    }


    /**
     * @brief Constructor (memory version).
     *
     * @param path Path to source file.
     */
    explicit Configuration(FilePath path) noexcept;


// Public Accessors
public:


    /**
     * @brief Returns of the source file.
     *
     * @return
     */
    const FilePath& getSourcePath() const noexcept
    {
        return m_filePath;
    }


    /**
     * @brief Returns if value exists under given name.
     *
     * @param name Value name.
     *
     * @return
     */
    bool has(const StringView& name) const noexcept
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
    String get(const StringView& name) const
    {
        if (!has(name))
            throw ConfigException("Missing value for '" + String(name) + "'");

        return m_impl->get(name);
    }


    /**
     * @brief Returns string value.
     *
     * @param name Value name.
     * @param def  Default value if doesn't exists.
     *
     * @return
     */
    String get(const StringView& name, String def) const noexcept
    {
        return has(name) ? m_impl->get(name) : std::move(def);
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
    T get(const StringView& name) const
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
    T get(const StringView& name, T def) const noexcept
    {
        return has(name) ? castFrom<T>(m_impl->get(name)) : std::move(def);
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
    bool hasConfiguration(const StringView& name) const noexcept
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
    Configuration getConfiguration(const StringView& name) const
    {
        auto configurations = getConfigurations(name);

        if (configurations.empty())
            throw ConfigException("Sub-configuration '" + String(name) + "' doesn't exists");

        return std::move(configurations[0]);
    }


    /**
     * @brief Returns all sub-configurations with given name.
     *
     * @param name Sub-configuration name.
     *
     * @return List of valid sub-configurations.
     */
    DynamicArray<Configuration> getConfigurations(const StringView& name) const noexcept
    {
        if (!hasConfiguration(name))
            return {};

        DynamicArray<Configuration> res;

        for (auto&& ptr : m_impl->getSubs(name))
            res.emplace_back(std::move(ptr), m_filePath);

        return res;
    }


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
    void set(const StringView& name, const StringView& value) noexcept
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
    void set(const String& name, T value) noexcept
    {
        m_impl->set(name, castTo(value));
    }


    /**
     * @brief Set content text.
     *
     * @param content Content text.
     */
    void setContent(const StringView& content) noexcept
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
    Configuration addConfiguration(const StringView& name) noexcept
    {
        return {m_impl->addSub(name), m_filePath};
    }


// Public Operations
public:


    /**
     * @brief Create usable path for filename in directory where the source
     * come from.
     *
     * @param filename
     *
     * @return
     */
    FilePath buildFilePath(const FilePath& filename) const noexcept;


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
        //return lexical_cast<T>(value);
        InStringStream is(value);
        T res;
        is >> std::noskipws >> res;
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
        //return lexical_cast<String>(value);
        OutStringStream os;
        os << value;
        return os.str();
    }


// Private Data Members
private:


    /// Configuration implementation.
    UniquePtr<Implementation> m_impl;

    /// Path to source configuration.
    FilePath m_filePath;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
