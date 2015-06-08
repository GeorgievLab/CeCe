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
#include "core/compatibility.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief GPU buffer.
 */
class DLL_EXPORT Buffer
{

// Public Types
public:


    /**
     * @brief Buffer ID type.
     */
    using Id = unsigned int;


    /**
     * @brief Buffer size type.
     */
    using SizeType = unsigned int;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit Buffer(Context& context);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param size    Buffer size.
     * @param data    Optional buffer data.
     */
    Buffer(Context& context, SizeType size, const void* data = nullptr);


    /**
     * @brief Destructor.
     */
    ~Buffer();


// Public Accessors
public:


    /**
     * @brief Return buffer ID.
     *
     * @return
     */
    Id getId() const NOEXCEPT
    {
        return m_id;
    }


    /**
     * @brief Return if buffer is initialized.
     *
     * @return
     */
    bool isInitialized() const NOEXCEPT
    {
        return m_id != 0;
    }


// Public Operators
public:


    /**
     * @brief Resize buffer object.
     *
     * @param context Rendering context.
     * @param size    Buffer size.
     * @param data    Optional buffer data.
     */
    void resize(SizeType size, const void* data = nullptr);


// Private Data Members
private:

    /// Buffer identifier.
    Id m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
