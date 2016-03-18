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

// Declaration
#include "cece/plugins/obstacle/Circle.hpp"

#ifdef CECE_ENABLE_BOX2D_PHYSICS
#  include <Box2D/Box2D.h>
#endif

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/Shape.hpp"
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace obstacle {

/* ************************************************************************ */

void Circle::configure(const config::Configuration& config, simulator::Simulation& simulation)
{
    object::Object::configure(config, simulation);

    // Init shape
    getMutableShapes().push_back(Shape::makeCircle(config.get<units::Length>("radius")));

    initShapes();
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Circle::draw(render::Context& context)
{
    if (!m_drawCircle)
        m_drawCircle.create(context);

    const auto& shape = getShapes().back();

    context.matrixPush();
    context.matrixTranslate(getPosition());
    context.matrixScale(shape.getCircle().radius / units::Length(1));
    m_drawCircle->draw(context, getColor());
    context.matrixPop();
}
#endif

/* ************************************************************************ */

void Circle::initShapes()
{
#ifdef CECE_ENABLE_BOX2D_PHYSICS
    const auto& shape = getShapes().back();
    m_bodyShape.m_radius = getConverter().convertLength(shape.getCircle().radius);
    getBody()->CreateFixture(&m_bodyShape, 1);
#endif
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
