
/* ************************************************************************ */

// Declaration
#include "simulator/World.hpp"

// C++
#include <algorithm>

// Simulator
#include "core/Matrix.hpp"
#include "simulator/StaticObject.hpp"
#include "simulator/DynamicObject.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World() noexcept
{
    // Nothing to do
}

/* ************************************************************************ */

World::~World()
{
    // Nothing to do
}

/* ************************************************************************ */

void World::clean()
{
    m_stepNumber = 0;
    m_objects.clear();
}

/* ************************************************************************ */

void World::reset()
{
    // Nothing to do yet
}

/* ************************************************************************ */

void World::update(units::Duration dt) noexcept
{
    // Increase step number
    m_stepNumber++;

    // Update all cells
    // Cell update can update cell list
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->update(dt);
    }

    // Remove cells that are outside world
    {
        const auto wh = getWidth() / 2.f;
        const auto hh = getWidth() / 2.f;

        // Kill objects that are outside world
        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [wh, hh](const ObjectContainer::value_type& obj) {
            if (!obj->hasFlag(OBJECT_STATIC))
                return false;

            // Cast to static object
            auto ptr = obj->cast<StaticObject>();

            // Get object position
            const Position& pos = ptr->getPosition();

            return not (
                ((pos.getX() >= -wh) && (pos.getX() <= wh)) &&
                ((pos.getY() >= -hh) && (pos.getY() <= hh))
            );
        }), m_objects.end());
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void World::drawInit(render::Context& context)
{
    // Prepare objects for rendering
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->drawInit(context);
    }
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void World::draw(render::Context& context, RenderFlagsType flags)
{
    // Prepare objects for rendering
    {
        for (auto ptr : m_drawInitList)
        {
            assert(ptr);
            ptr->drawInit(context);
        }

        m_drawInitList.clear();
    }

    // Draw lines around world
    {
        const auto hw = getWidth() * 0.5f;
        const auto hh = getHeight() * 0.5f;
        const auto sw = getWidth() / 16.f;
        const auto sh = getHeight() / 16.f;

        const render::Color color = {0.3, 0.3, 0.3, 0.2};

        context.drawLine({-hw, -hh}, {0, sh}, color);
        context.drawLine({-hw, -hh}, {sw, 0}, color);

        context.drawLine({hw, -hh}, {0, sh}, color);
        context.drawLine({hw, -hh}, {-sw, 0}, color);

        context.drawLine({-hw, hh}, {0, -sh}, color);
        context.drawLine({-hw, hh}, {sw, 0}, color);

        context.drawLine({hw, hh}, {0, -sh}, color);
        context.drawLine({hw, hh}, {-sw, 0}, color);
    }

    // Draw objects
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->draw(context);
    }
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
