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
#include "cece/render/Shader.hpp"

/* ************************************************************************ */

namespace cece {
namespace render {

/* ************************************************************************ */

/**
 * @brief OpenGL program object.
 */
class CECE_EXPORT Program
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
}

/* ************************************************************************ */
