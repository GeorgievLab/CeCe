/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
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

    auto& shapes = getMutableShapes();
    assert(!shapes.empty());
    auto& shape = shapes[0];

    // Obstacle shape
    auto type = config.get("type");

    if (type == "circle")
        shape.type = ShapeType::Circle;
    else if (type == "rectangle")
        shape.type = ShapeType::Rectangle;
    // TODO: other shapes

    // Different configurations for different types
    switch (shape.type)
    {
    case ShapeType::Circle:
        shape.circle.radius = config.get<units::Length>("radius");
        break;

    case ShapeType::Rectangle:
        shape.rectangle.size = config.get<PositionVector>("size");
        break;

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
    // Foreach shapes
    for (const auto& shape : getShapes())
    {
        switch (shape.type)
        {
        case ShapeType::Circle:
            if (!m_drawCircle)
                m_drawCircle.create(context);

            context.matrixPush();
            context.matrixTranslate(getPosition());
            context.matrixScale(shape.circle.radius / units::Length(1));
            m_drawCircle->draw(context);
            context.matrixPop();
            break;

        case ShapeType::Rectangle:
            if (!m_drawRectangle)
                m_drawRectangle.create(context);

            context.matrixPush();
            context.matrixTranslate(getPosition());
            context.matrixScale(shape.rectangle.size / units::Length(1));
            m_drawRectangle->draw(context);
            context.matrixPop();
            break;

        case ShapeType::Undefined:
            assert(false || "This shouldn't happend");
            break;
        }
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
        switch (shape.type)
        {
        case ShapeType::Circle:
        {
            auto* s = new b2CircleShape{};
            s->m_radius = shape.circle.radius.value();
            m_bodyShapes.emplace_back(s);
            getBody()->CreateFixture(s, 1);
            break;
        }
        case ShapeType::Rectangle:
        {
            const auto sh = shape.rectangle.size / 2.f;
            auto* s = new b2PolygonShape{};
            s->SetAsBox(sh.getWidth().value(), sh.getHeight().value());
            m_bodyShapes.emplace_back(s);
            getBody()->CreateFixture(s, 1);
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
