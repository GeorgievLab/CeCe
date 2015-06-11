
// Declaration
#include "Module.hpp"

// C++
#include <random>

// Simulator
#include "core/VectorRange.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace module {
namespace cylinder_streamlines {

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    if (m_update)
    {
        // Precompute values
        const Vector<float> start = simulation.getWorldSize() * -0.5f;
        const auto gridSize = m_grid.getSize();
        const Vector<float> step = simulation.getWorldSize() / gridSize;

        float radius = 0.f;

        if (m_object)
        {
            const auto& shapes = m_object->getShapes();
            assert(shapes.size());
            const simulator::Shape& shape = shapes[0];
            assert(shape.type == simulator::ShapeType::Circle);
            radius = 2 * shape.circle.radius;
        }

        const auto R2 = radius * radius;

        // Foreach all combination in range [0, 0] - gridSize
        for (auto&& c : range(gridSize))
        {
            auto& velocity = m_grid[c];

            if (!m_object)
            {
                velocity = VelocityVector{1.f, 0.f};
                continue;
            }

            // Transform i, j coordinates to position
            // Cell center position
            const auto coord = PositionVector(c) + 0.5f;
            // Real position in the world
            const auto pos = start + step * coord - m_object->getPosition();

            // Calculate squared distance from main cell
            const auto distSq = pos.getLengthSquared();

            // Cell is in main cell, ignore
            if (distSq <= R2)
            {
                velocity = VelocityVector::Zero;
                continue;
            }
/*
            // Precompute values
            const float distQuad = distSq * distSq;
            const float xx = pos.x * pos.x;
            const float xy = pos.x * pos.y;

            // COPYRIGHT: Hynek magic
            cell.velocity.x = U * (1 + R2 / distSq - 2 * (xx * R2) / distQuad);
            cell.velocity.y = U * -2 * (R2 * xy) / distQuad;
*/
            const float theta = atan2(pos.getY(), pos.getX());

            const auto u = VelocityVector{
                cosf(theta) * (1 - R2 / distSq),
                -sinf(theta) * (1 + R2 / distSq)
            };

            velocity = u.rotated(theta);
            //cell.velocity = u;
        }

        m_update = false;
        m_renderUpdate = true;
    }

    // Apply streamlines to world objects
    applyToObjects(simulation);
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Grid size
    config.callIfSetString("grid", [this](const String& value) {
        m_grid.resize(parser::parse_vector_single<SizeType>(value));
    });

    // Flow velocity
    config.callIfSetString("flow-velocity", [this](const String& value) {
        setFlowSpeed(parser::parse_value<float>(value));
    });

    // Create object
    config.callIfSetString("object", [this, &config, &simulation](const String& value) mutable {
        m_object = simulation.buildObject(value, false);
        if (m_object)
            m_object->configure(config, simulation);
    });

#ifdef ENABLE_RENDER
    // Draw flags
    config.callIfSetString("draw-velocity", [this](const String& value) {
        m_drawObject = parser::parse_bool(value);
    });
#endif
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    if (!m_drawObject)
        return;

    if (!m_renderObject)
    {
        m_renderObject.create(context, m_grid.getSize(), m_grid.getData());
    }
    else if (m_renderUpdate)
    {
        m_renderObject->update(m_grid.getData());
    }

    context.matrixPush();
    context.matrixScale(simulation.getWorldSize());
    m_renderObject->draw(context);
    context.matrixPop();

    m_renderUpdate = false;
}
#endif

/* ************************************************************************ */

void Module::applyToObjects(const simulator::Simulation& simulation)
{
    // Get grid
    const auto& grid = getGrid();

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / grid.getSize();

    // Foreach objects
    for (auto& obj : simulation.getObjects())
    {
        // Ignore static objects
        if (obj->getType() == simulator::Object::Type::Static)
            continue;

        // Get position
        const auto pos = obj->getPosition() - start;

        // Check if object is in range
        if (!pos.inRange(PositionVector::Zero, simulation.getWorldSize()))
            continue;

        // Get grid position
        const auto coord = Coordinate(pos / step);

        // Set object velocity
        obj->setVelocity(grid[coord] * getFlowSpeed());
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
