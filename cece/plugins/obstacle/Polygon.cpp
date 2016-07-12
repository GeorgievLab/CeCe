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
#include "cece/plugins/obstacle/Polygon.hpp"

// CeCe
#include "cece/core/VectorUnits.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/core/Shape.hpp"
#include "cece/core/StringStream.hpp"
#include "cece/config/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace obstacle {

/* ************************************************************************ */

void Polygon::configure(const config::Configuration& config, simulator::Simulation& simulation)
{
    object::Object::configure(config, simulation);

    DynamicArray<units::PositionVector> edges;
    InStringStream iss(config.get("vertices"));
    units::PositionVector vector;

    while (iss >> vector)
        edges.push_back(vector);

    // Init shape
    getMutableShapes().push_back(Shape::makeEdges(std::move(edges)));

    initShapes();
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Polygon::draw(render::Context& context)
{
    const RenderState& state = m_drawableState.getFront();

    if (!m_drawPolygon)
        m_drawPolygon.create(context, state.lines);

    context.matrixPush();
    context.matrixTranslate(state.position);
    m_drawPolygon->draw(context, state.color);
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Polygon::drawStoreState()
{
    RenderState& state = m_drawableState.getBack();

    state.position = getPosition();
    state.color = getColor();

    for (const auto& shape : getShapes())
    {
        const auto& edges = shape.getEdges().edges;
        state.lines.reserve(state.lines.size() + edges.size());

        // Create lines from shape
        for (std::size_t i = 1u; i < edges.size(); ++i)
        {
            state.lines.push_back(makePair(
                render::Lines::PointType(edges[i - 1].getX().value(), edges[i - 1].getY().value()),
                render::Lines::PointType(edges[i].getX().value(), edges[i].getY().value())
            ));
        }

        if (edges.size() > 2)
        {
            state.lines.push_back(makePair(
                render::Lines::PointType(edges.back().getX().value(), edges.back().getY().value()),
                render::Lines::PointType(edges.front().getX().value(), edges.front().getY().value())
            ));
        }
    }
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Polygon::drawSwapState()
{
    m_drawableState.swap();
}
#endif

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
