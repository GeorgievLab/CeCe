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
#include "parser/Parser.hpp"
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
    config.callString("type", [&shape](const String& value) {
        if (value == "circle")
            shape.type = ShapeType::Circle;
        // TODO: other shapes
    });

    // Different configurations for different types
    switch (shape.type)
    {
    case ShapeType::Circle:
        config.callString("radius", [&shape](const String& value) {
            shape.circle.radius = parser::parse_value<units::Length>(value);
        });
        break;
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
            context.matrixScale(shape.circle.radius.value());
            m_drawCircle->draw(context);
            context.matrixPop();
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
        }
    }
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
