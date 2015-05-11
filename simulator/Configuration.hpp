
#pragma once

/* ************************************************************************ */

// C++
#include <string>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Base class for configuration storages.
 */
class ConfigurationBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~ConfigurationBase()
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
    virtual const std::string& getSourcePath() const noexcept = 0;


    /**
     * @brief Returns if value exists under given name.
     *
     * @param name
     *
     * @return
     */
    virtual bool hasValue(const std::string& name) const = 0;


    /**
     * @brief Returns string value.
     *
     * @param name
     *
     * @return
     */
    virtual std::string getString(const std::string& name) const = 0;


    /**
     * @brief Returns integer value.
     *
     * @param name
     *
     * @return
     */
    virtual int getInteger(const std::string& name) const = 0;


    /**
     * @brief Returns float value.
     *
     * @param name
     *
     * @return
     */
    virtual float getFloat(const std::string& name) const = 0;


// Public Mutators
public:


    /**
     * @brief Set string value.
     *
     * @param name
     * @param value
     */
    virtual void setString(const std::string& name, const std::string& value) = 0;


    /**
     * @brief Set integer value.
     *
     * @param name
     * @param value
     */
    virtual void setInteger(const std::string& name, int value) = 0;


    /**
     * @brief Set float value.
     *
     * @param name
     * @param value
     */
    virtual void setFloat(const std::string& name, float value) = 0;


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
    std::string buildFilePath(const std::string& filename) const noexcept;


    /**
     * @brief Call given function when configuration under name is set (not empty).
     *
     * @tparam Fn Function type.
     *
     * @param name
     * @param fn
     */
    template<typename Fn>
    void callIfSetString(const std::string& name, Fn fn) const
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
    void callIfSetInteger(const std::string& name, Fn fn) const
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
    void callIfSetFloat(const std::string& name, Fn fn) const
    {
        if (hasValue(name))
            fn(getFloat(name));
    }
};

/* ************************************************************************ */

}

/* ************************************************************************ */
