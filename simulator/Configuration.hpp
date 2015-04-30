
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

};

/* ************************************************************************ */

}

/* ************************************************************************ */
