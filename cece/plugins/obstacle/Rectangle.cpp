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

// Declaration
#include "cece/plugins/obstacle/Rectangle.hpp"

// CeCe
#include "cece/core/UnitIo.hpp"
#include "cece/core/Shape.hpp"
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace obstacle {

/* ************************************************************************ */

void Rectangle::configure(const config::Configuration& config, simulator::Simulation& simulation)
{
    object::Object::configure(config, simulation);

    // Init shape
    getMutableShapes().push_back(Shape::makeRectangle(config.get<units::PositionVector>("size")));

    initShapes();
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Rectangle::draw(render::Context& context)
{
    if (!m_drawRectangle)
        m_drawRectangle.create(context);

    const RenderState& state = m_drawableState.getFront();

    context.matrixPush();
    context.matrixTranslate(state.position);
    context.matrixScale(state.size / units::Length(1));
    m_drawRectangle->draw(context, state.color);
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Rectangle::drawStoreState()
{
    RenderState& state = m_drawableState.getBack();
    const auto& shape = getShapes().back();

    state.position = getPosition();
    state.size = shape.getRectangle().size;
    state.color = getColor();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Rectangle::drawSwapState()
{
    m_drawableState.swap();
}
#endif

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
