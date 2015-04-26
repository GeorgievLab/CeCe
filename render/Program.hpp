
#pragma once

/* ************************************************************************ */

// Simulator
#include "render/Shader.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL program object.
 */
class Program
{

// Public Types
public:


    /**
     * @brief Buffer ID type.
     */
    using Id = unsigned int;


    /**
     * @brief Uniform variable ID type.
     */
    using UniformId = unsigned int;


// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Program();


// Public Accessors
public:


    /**
     * @brief Return program ID.
     *
     * @return
     */
    Id getId() const noexcept
    {
        return m_id;
    }


    /**
     * @brief Return uniform variable ID.
     *
     * @param name
     *
     * @return
     */
    UniformId getUniformId(const char* name) const noexcept;


// Public Operators
public:


    /**
     * @brief Initializer.
     *
     * @param vs Vertex shader.
     * @param fs Fragment shader.
     */
    void init(const Shader& vs, const Shader& fs);


// Private Data Members
private:

    /// Program identifier.
    Id m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
