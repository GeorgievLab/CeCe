/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "simulator/Obstacle.hpp"

// C++
#include <cassert>

// Simulator
#include "core/VectorUnits.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

void Obstacle::configure(const Configuration& config, Simulation& simulation)
{
    Object::configure(config, simulation);

    // Draw configured
    m_draw = config.get("show", false);

    auto& shapes = getMutableShapes();
    assert(!shapes.empty());
    auto& shape = shapes[0];

    // Obstacle shape
    auto type = config.get("type");

    // Shape type
    ShapeType shapeType = ShapeType::Undefined;

    if (type == "circle")
        shapeType = ShapeType::Circle;
    else if (type == "rectangle")
        shapeType = ShapeType::Rectangle;
    else if (type == "polygon")
        shapeType = ShapeType::Edges;
    // TODO: other shapes

    // Different configurations for different types
    switch (shapeType)
    {
    case ShapeType::Circle:
        shape = Shape::makeCircle(config.get<units::Length>("radius"));
        break;

    case ShapeType::Rectangle:
        shape = Shape::makeRectangle(config.get<PositionVector>("size"));
        break;

    case ShapeType::Edges:
    {
        DynamicArray<PositionVector> edges;
        InStringStream iss(config.get("vertices"));
        PositionVector vector;

        while (iss >> vector)
            edges.push_back(vector);

        shape = Shape::makeEdges(std::move(edges));
        break;
    }

    case ShapeType::Undefined:
        throw RuntimeException("Unknown shape type");
    }

#if ENABLE_PHYSICS
    initShapes();
#endif
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Obstacle::draw(render::Context& context)
{
    // TODO: redesign

    if (!m_draw)
        return;

    DynamicArray<render::Lines::LineType> lines;

    // Foreach shapes
    for (const auto& shape : getShapes())
    {
        switch (shape.getType())
        {
        case ShapeType::Circle:
            if (!m_drawCircle)
                m_drawCircle.create(context);

            context.matrixPush();
            context.matrixTranslate(getPosition());
            context.matrixScale(shape.getCircle().radius / units::Length(1));
            m_drawCircle->draw(context);
            context.matrixPop();
            break;

        case ShapeType::Rectangle:
            if (!m_drawRectangle)
                m_drawRectangle.create(context);

            context.matrixPush();
            context.matrixTranslate(getPosition());
            context.matrixScale(shape.getRectangle().size / units::Length(1));
            m_drawRectangle->draw(context);
            context.matrixPop();
            break;

        case ShapeType::Edges:
            if (!m_drawEdges)
            {
                const auto& edges = shape.getEdges().edges;
                lines.reserve(lines.size() + edges.size());

                // Create lines from shape
                for (std::size_t i = 1u; i < edges.size(); ++i)
                {
                    lines.push_back(makePair(
                        render::Lines::PointType(edges[i - 1].getX().value(), edges[i - 1].getY().value()),
                        render::Lines::PointType(edges[i].getX().value(), edges[i].getY().value())
                    ));
                }

                if (edges.size() > 2)
                {
                    lines.push_back(makePair(
                        render::Lines::PointType(edges.back().getX().value(), edges.back().getY().value()),
                        render::Lines::PointType(edges.front().getX().value(), edges.front().getY().value())
                    ));
                }
            }

        case ShapeType::Undefined:
            assert(false || "This shouldn't happend");
            break;
        }
    }

    if (!m_drawEdges)
    {
        if (!lines.empty())
            m_drawEdges.create(context, std::move(lines));
    }
    else
    {
        context.matrixPush();
        context.matrixTranslate(getPosition());
        m_drawEdges->draw(context);
        context.matrixPop();
    }
}
#endif

/* ************************************************************************ */

#if ENABLE_PHYSICS
void Obstacle::initShapes()
{
    assert(getBody()->GetFixtureList() == nullptr);
    assert(m_bodyShapes.empty());

    // Foreach shapes
    for (const auto& shape : getShapes())
    {
        switch (shape.getType())
        {
        case ShapeType::Circle:
        {
            auto s = makeUnique<b2CircleShape>();
            s->m_radius = shape.getCircle().radius.value();
            getBody()->CreateFixture(s.get(), 1);
            m_bodyShapes.emplace_back(s.release());
            break;
        }

        case ShapeType::Rectangle:
        {
            const auto sh = shape.getRectangle().size / 2.f;
            auto s = makeUnique<b2PolygonShape>();
            s->SetAsBox(sh.getWidth().value(), sh.getHeight().value());
            getBody()->CreateFixture(s.get(), 1);
            m_bodyShapes.emplace_back(s.release());
            break;
        }

        case ShapeType::Edges:
        {
            DynamicArray<b2Vec2> vertices;

            auto s = makeUnique<b2ChainShape>();
            for (const auto& v : shape.getEdges().edges)
                vertices.push_back(b2Vec2(v.getX().value(), v.getY().value()));

            // Create edges loop
            if (vertices.size() < 3)
                s->CreateChain(vertices.data(), vertices.size());
            else
                s->CreateLoop(vertices.data(), vertices.size());

            getBody()->CreateFixture(s.get(), 1);
            m_bodyShapes.emplace_back(s.release());
            break;
        }

        case ShapeType::Undefined:
            assert(false || "This shouldn't happend");
            break;
        }
    }
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
