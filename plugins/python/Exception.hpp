
#pragma once

/* ************************************************************************ */

// C++
#include <string>
#include <exception>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

/**
 * @brief Python module exception.
 */
class Exception : public std::exception
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Exception();


// Public Accessors
public:


    /**
     * @brief Returns the explanatory string.
     */
    const char* what() const NOEXCEPT
    {
        return m_message.c_str();
    }


// Private Data Members
private:


    /// Error message.
    std::string m_message;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
