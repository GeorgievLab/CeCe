/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/export.hpp"
#include "cece/core/Vector.hpp"
#include "cece/render/Color.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Texture object.
 */
class CECE_EXPORT Texture
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
}

/* ************************************************************************ */
