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
class Texture
{

// Public Types
public:


    /**
     * @brief Texture ID type.
     */
    using Id = unsigned int;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param filter  Min/Mag filter.
     */
    explicit Texture(Context& context, bool filter = true);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param size    Texture size.
     * @param color   Initialization color.
     * @param filter  Min/Mag filter.
     */
    Texture(Context& context, Size size, const Color& color = colors::BLACK, bool filter = true);


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
    Id getId() const noexcept
    {
        return m_id;
    }


    /**
     * @brief Returns texture size.
     *
     * @return
     */
    const Size& getSize() const noexcept
    {
        return m_size;
    }


    /**
     * @brief Return if buffer is initialized.
     *
     * @return
     */
    bool isInitialized() const noexcept
    {
        return m_id != 0;
    }


// Public Operations
public:


    /**
     * @brief Fill texture with given color.
     *
     * @param color   Initialization color.
     */
    void clear(const Color& color = colors::BLACK);


    /**
     * @brief Resize texture object.
     *
     * @param context Rendering context.
     * @param size    Texture size.
     * @param color   Initialization color.
     */
    void resize(Size size, const Color& color = colors::BLACK);


    /**
     * @brief Update texture.
     *
     * @param colors New texture data.
     */
    void update(const Color* colors);


    /**
     * @brief (Re)create texture object.
     *
     * @param size    Texture size.
     * @param color   Initialization color.
     */
    void create(Size size, const Color* colors);


    /**
     * @brief (Re)create texture object.
     *
     * @param size   Texture size.
     * @param colors Texture colors.
     */
    void createGray(Size size, const unsigned char* colors);


// Private Data Members
private:

    /// Texture identifier.
    Id m_id = 0;

    /// Texture size.
    Size m_size;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
