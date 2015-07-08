/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/FilePath.hpp"
#include "core/Exception.hpp"
#include "core/DynamicArray.hpp"

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
 * @brief Base class for configuration storages.
 */
class DLL_EXPORT Configuration
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~Configuration()
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
    virtual const FilePath& getSourcePath() const NOEXCEPT = 0;


    /**
     * @brief Returns if value exists under given name.
     *
     * @param name
     *
     * @return
     */
    virtual bool hasValue(const String& name) const = 0;


    /**
     * @brief Returns string value.
     *
     * @param name
     *
     * @return
     */
    virtual String getString(const String& name) const = 0;


    /**
     * @brief Returns integer value.
     *
     * @param name
     *
     * @return
     */
    virtual int getInteger(const String& name) const = 0;


    /**
     * @brief Returns float value.
     *
     * @param name
     *
     * @return
     */
    virtual float getFloat(const String& name) const = 0;


    /**
     * @brief Returns if there is content text.
     *
     * @return
     */
    virtual bool hasText() const = 0;


    /**
     * @brief Returns text string.
     *
     * @return
     */
    virtual String getText() const = 0;


    /**
     * @brief Returns configuration subconfiguration.
     *
     * @param name Configuration name.
     *
     * @return Configuration or nullptr.
     */
    virtual Configuration* getConfiguration(const String& name) const noexcept
    {
        return nullptr;
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
        return {};
    }


// Public Mutators
public:


    /**
     * @brief Set string value.
     *
     * @param name
     * @param value
     */
    virtual void setString(const String& name, const String& value) = 0;


    /**
     * @brief Set integer value.
     *
     * @param name
     * @param value
     */
    virtual void setInteger(const String& name, int value) = 0;


    /**
     * @brief Set float value.
     *
     * @param name
     * @param value
     */
    virtual void setFloat(const String& name, float value) = 0;


    /**
     * @brief Set text.
     *
     * @param text
     */
    virtual void setText(String text) = 0;


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
    FilePath buildFilePath(const FilePath& filename) const NOEXCEPT;


    /**
     * @brief Call given function when configuration under name is set (not empty).
     *
     * @tparam Fn Function type.
     *
     * @param name
     * @param fn
     */
    template<typename Fn>
    void callIfSetString(const String& name, Fn fn) const
    {
        if (hasValue(name))
            fn(getString(name));
    }


    /**
     * @brief Call given function when configuration under name is set (not empty).
     *
     * @tparam Fn Function type.
     *
     * @param name
     * @param fn
     */
    template<typename Fn>
    void callString(const String& name, Fn fn) const
    {
        if (hasValue(name))
            fn(getString(name));
        else
            throw ConfigException("Missing configuration '" + name + "'");
    }


    /**
     * @brief Call given function when configuration under name is set (not empty).
     *
     * @tparam Fn Function type.
     *
     * @param name
     * @param fn
     */
    template<typename Fn>
    void callIfSetInteger(const String& name, Fn fn) const
    {
        if (hasValue(name))
            fn(getInteger(name));
    }


    /**
     * @brief Call given function when configuration under name is set (not empty).
     *
     * @tparam Fn Function type.
     *
     * @param name
     * @param fn
     */
    template<typename Fn>
    void callInteger(const String& name, Fn fn) const
    {
        if (hasValue(name))
            fn(getInteger(name));
        else
            throw ConfigException("Missing configuration '" + name + "'");
    }


    /**
     * @brief Call given function when configuration under name is set (not empty).
     *
     * @tparam Fn Function type.
     *
     * @param name
     * @param fn
     */
    template<typename Fn>
    void callIfSetFloat(const String& name, Fn fn) const
    {
        if (hasValue(name))
            fn(getFloat(name));
    }


    /**
     * @brief Call given function when configuration under name is set (not empty).
     *
     * @tparam Fn Function type.
     *
     * @param name
     * @param fn
     */
    template<typename Fn>
    void callFloat(const String& name, Fn fn) const
    {
        if (hasValue(name))
            fn(getFloat(name));
        else
            throw ConfigException("Missing configuration '" + name + "'");
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
