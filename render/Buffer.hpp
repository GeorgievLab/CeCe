
#pragma once

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL buffer.
 */
class Buffer
{

// Public Types
public:


    /**
     * @brief Buffer ID type.
     */
    using Id = unsigned int;


// Public Ctors & Dtors
public:


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
    Id getId() const noexcept
    {
        return m_id;
    }


// Public Operators
public:


    /**
     * @brief Initialize buffer object.
     */
    void init();


// Private Data Members
private:

    /// Buffer identifier.
    Id m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
