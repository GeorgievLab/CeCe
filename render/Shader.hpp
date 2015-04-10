
#pragma once

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL shader object.
 */
class Shader
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
    Id getId() const noexcept
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


// Private Data Members
private:

    /// Shader identifier.
    Id m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
