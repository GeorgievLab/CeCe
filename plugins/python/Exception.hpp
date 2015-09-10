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
#include "core/String.hpp"
#include "core/Exception.hpp"

/* ************************************************************************ */

namespace plugin {
namespace python {

/* ************************************************************************ */

/**
 * @brief Python plugin exception.
 */
class Exception : public core::Exception
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Exception();


    /**
     * @brief Constructor.
     *
     * @param error Error message.
     */
    explicit Exception(const char* error);


// Public Accessors
public:


    /**
     * @brief Returns the explanatory string.
     */
    const char* what() const noexcept
    {
        return m_message.c_str();
    }


// Private Data Members
private:


    /// Error message.
    String m_message;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
