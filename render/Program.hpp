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
#include "render/Shader.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL program object.
 */
class DLL_EXPORT Program
{

// Public Types
public:


    /**
     * @brief Program ID type.
     */
    using Id = unsigned int;


    /**
     * @brief Uniform variable ID type.
     */
    using UniformId = int;


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
    Id getId() const NOEXCEPT
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
    UniformId getUniformId(const char* name) const NOEXCEPT;


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
