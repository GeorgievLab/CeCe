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
#include "cece/core/StaticArray.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL shader object.
 */
class CECE_EXPORT Shader
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


    /**
     * @brief Init shader.
     *
     * @param type   Type of the shader.
     * @param source Shader source.
     */
    template<std::size_t N>
    void init(Type type, const StaticArray<unsigned char, N>& source)
    {
        init(type, reinterpret_cast<const char*>(source.data()), N);
    }


// Private Data Members
private:

    /// Shader identifier.
    Id m_id = 0;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
