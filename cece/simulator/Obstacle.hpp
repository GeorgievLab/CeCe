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
#include "cece/simulator/Object.hpp"
#include "cece/simulator/Shape.hpp"

#ifdef CECE_ENABLE_RENDER
#  include "cece/render/Object.hpp"
#  include "cece/render/Circle.hpp"
#  include "cece/render/Rectangle.hpp"
#  include "cece/render/Lines.hpp"
#endif

#ifdef CECE_ENABLE_BOX2D_PHYSICS
#  include <Box2D/Box2D.h>
#endif

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

class Simulation;
class Configuration;

/* ************************************************************************ */

/**
 * @brief Physical obstacle.
 */
class Obstacle : public Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation Simulation object reference.
     */
    explicit Obstacle(Simulation& simulation) noexcept
        : Object(simulation, "simulator.Obstacle", Type::Static)
    {
        getMutableShapes().push_back(Shape::makeUndefined());
    }


// Public Operations
public:


    /**
     * @brief Configure obstacle.
     *
     * @param config
     * @param simulation
     */
    void configure(const Configuration& config, Simulation& simulation) override;


#ifdef CECE_ENABLE_RENDER

    /**
     * @brief Render obstacle.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;

#endif


#ifdef CECE_ENABLE_BOX2D_PHYSICS

    /**
     * @brief Init obstacle physics shapes.
     */
    void initShapes();

#endif


// Private Data Members
private:

#ifdef CECE_ENABLE_BOX2D_PHYSICS

    /// Physical body shapes.
    DynamicArray<UniquePtr<b2Shape>> m_bodyShapes;

#endif

#ifdef CECE_ENABLE_RENDER

    /// Shared object for drawing circle.
    render::ObjectSharedPtr<render::Circle> m_drawCircle;

    /// Shared object for drawing rectangle.
    render::ObjectSharedPtr<render::Rectangle> m_drawRectangle;

    /// Object for drawing lines
    render::ObjectPtr<render::Lines> m_drawEdges;

#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
