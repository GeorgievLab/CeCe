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
#include "core/StaticArray.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL shader object.
 */
class DLL_EXPORT Shader
{

// Public Types
public:


    /**
     * @brief Buffer ID type.
     */
    using Id = unsigned int;


// Public Enums
public:


    /**
     * @brief Shader types.
     */
    enum class Type
    {
        VERTEX,
        FRAGMENT
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    ~Shader();


// Public Accessors
public:


    /**
     * @brief Return shader identifier.
     *
     * @return
     */
    Id getId() const NOEXCEPT
    {
        return m_id;
    }


// Public Operations
public:


    /**
     * @brief Init shader.
     *
     * @param type   Type of the shader.
     * @param source Shader source.
     * @param length Length of the shader source.
     */
    void init(Type type, const char* source, unsigned length);


    /**
     * @brief Init shader.
     *
     * @param type   Type of the shader.
     * @param source Shader source.
     */
    template<std::size_t N>
    void init(Type type, const StaticArray<char, N>& source)
    {
        init(type, source.data(), N);
    }


// Private Data Members
private:

    /// Shader identifier.
    Id m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
