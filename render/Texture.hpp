
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>

// Simulator
#include "core/compatibility.hpp"
#include "core/Vector.hpp"
#include "render/Color.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Texture object.
 */
class DLL_EXPORT Texture
{

// Public Types
public:


    /**
     * @brief Texture ID type.
     */
    using Id = unsigned int;


    /**
     * @brief Texture size type.
     */
    using SizeType = unsigned int;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     */
    explicit Texture(Context& context);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param size    Texture size.
     * @param color   Initialization color.
     */
    Texture(Context& context, Vector<SizeType> size, const Color& color = colors::BLACK);


    /**
     * @brief Destructor.
     */
    ~Texture();


// Public Accessors
public:


    /**
     * @brief Return texture ID.
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


// Public Operations
public:


    /**
     * @brief Resize texture object.
     *
     * @param context Rendering context.
     * @param size    Texture size.
     * @param color   Initialization color.
     */
    void resize(Vector<SizeType> size, const Color& color = colors::BLACK);


    /**
     * @brief Update texture.
     *
     * @param size   Texture data size.
     * @param colors New texture data.
     */
    void update(const Vector<SizeType>& size, const Color* colors);


// Private Data Members
private:

    /// Texture identifier.
    Id m_id = 0;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
