
// Declaration
#include "Module.hpp"

// C++
#include <random>

// Simulator
#include "simulator/Simulation.hpp"
#include "simulator/Object.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace module {
namespace cylinder_streamlines {

/* ************************************************************************ */

void Module::update(core::units::Duration dt, simulator::Simulation& simulation)
{
    if (m_update)
    {
        // Precompute values
        const core::Vector<float> start = simulation.getWorldSize() * -0.5f;
        const auto gridSize = m_grid.getSize();
        const core::Vector<float> step = simulation.getWorldSize() / gridSize;

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

        for (decltype(gridSize.getHeight()) j = 0; j < gridSize.getHeight(); ++j)
        {
            for (decltype(gridSize.getWidth()) i = 0; i < gridSize.getWidth(); ++i)
            {
                auto& velocity = m_grid[core::Vector<SizeType>(i, j)];

                if (!m_object)
                {
                    velocity = core::Vector<float>{1.f, 0.f};
                    continue;
                }

                // Transform i, j coordinates to position
                // Cell center position
                const core::Vector<float> coord = core::Vector<float>(i, j) + 0.5f;
                // Real position in the world
                const core::Vector<float> pos = start + step * coord - m_object->getPosition();

                // Calculate squared distance from main cell
                const auto distSq = pos.getLengthSquared();

                // Cell is in main cell, ignore
                if (distSq <= R2)
                {
                    velocity = core::Vector<float>{0.f, 0.f};
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

                const core::Vector<float> u = core::Vector<float>{
                    cosf(theta) * (1 - R2 / distSq),
                    -sinf(theta) * (1 + R2 / distSq)
                };

                velocity = u.rotated(theta);
                //cell.velocity = u;
            }
        }

        m_update = false;
        m_renderUpdate = true;
    }

    // Apply streamlines to world objects
    {
        // Get grid
        const auto& grid = getGrid();

        const core::Vector<float> start = simulation.getWorldSize() * -0.5;
        const auto step = simulation.getWorldSize() / grid.getSize();

        for (auto& obj : simulation.getObjects())
        {
            // Ignore static objects
            if (obj->getType() == simulator::Object::Type::Static)
                continue;

            // Get position
            const auto pos = obj->getPosition() - start;

            // Check if object is in range
            if (!pos.inRange(core::Vector<float>{0}, simulation.getWorldSize()))
                continue;

            // Get grid position
            core::Vector<SizeType> coord = pos / step;

            // Get velocity
            const auto velocity = grid[coord] * m_flowSpeed;

            // Set object velocity
            obj->setVelocity(velocity);
        }
    }
}

/* ************************************************************************ */

void Module::configure(const simulator::ConfigurationBase& config, simulator::Simulation& simulation)
{
    // Grid size
    config.callIfSetString("grid", [this](const std::string& value) {
        m_grid.resize(parser::parse_vector_single<SizeType>(value));
    });

    // Flow speed
    config.callIfSetString("flow-speed", [this](const std::string& value) {
        setFlowSpeed(parser::parse_value<float>(value));
    });

    // Create object
    config.callIfSetString("object", [this, &config, &simulation](const std::string& value) mutable {
        m_object = simulation.buildObject(value, false);
        if (m_object)
            m_object->configure(config, simulation);
    });

#ifdef ENABLE_RENDER
    // Draw flags
    config.callIfSetString("draw-velocity", [this](const std::string& value) {
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

}
}

/* ************************************************************************ */
