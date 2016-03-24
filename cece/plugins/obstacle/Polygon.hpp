/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/core/DynamicArray.hpp"
#include "cece/core/UniquePtr.hpp"
#include "cece/object/Object.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Object.hpp"
#  include "cece/render/Lines.hpp"
#endif

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace obstacle {

/* ************************************************************************ */

/**
 * @brief Polygon obstacle.
 */
class Polygon : public object::Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation Simulation object reference.
     */
    explicit Polygon(simulator::Simulation& simulation, String typeName = "obstacle.Polygon",
                   object::Object::Type type = object::Object::Type::Static) noexcept
        : object::Object(simulation, typeName, object::Object::Type::Static)
    {
        // Nothing to do
    }



// Public Operations
public:


    /**
     * @brief Configure obstacle.
     *
     * @param config
     * @param simulation
     */
    void configure(const config::Configuration& config, simulator::Simulation& simulation) override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render obstacle.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;

#endif


// Private Data Members
private:

#ifdef CECE_ENABLE_RENDER

    /// Shared object for drawing polygon.
    render::ObjectPtr<render::Lines> m_drawPolygon;

#endif
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */

