
/* ************************************************************************ */

// Declaration
#include "simulator/Simulation.hpp"

// C++
#include <algorithm>

// Simulator
#include "simulator/StaticObject.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Simulation::Simulation() noexcept
{
    // Nothing to do
}

/* ************************************************************************ */

Simulation::~Simulation()
{
    // Nothing to do
}

/* ************************************************************************ */

void Simulation::reset()
{
    m_stepNumber = 0;
}

/* ************************************************************************ */

void Simulation::update(units::Duration dt) noexcept
{
    // Increase step number
    m_stepNumber++;

    // Update modules
    for (auto& module : getModules())
        module.second->update(dt, *this);

    // Update simulations objects
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->update(dt);
    }

    // Remove objects that are outside world.
    {
        const auto hh = getWorldSize() * 0.5f;

        // Kill objects that are outside world
        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [&hh](const ObjectContainer::value_type& obj) {
            if (!obj->hasFlag(OBJECT_STATIC))
                return false;

            // Cast to static object
            auto ptr = obj->cast<StaticObject>();

            // Get object position
            const Position& pos = ptr->getPosition();

            // TODO: optimize
            return not (
                ((pos.getX() >= -hh.getX()) && (pos.getX() <= hh.getX())) &&
                ((pos.getY() >= -hh.getY()) && (pos.getY() <= hh.getY()))
            );
        }), m_objects.end());
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Simulation::drawInit(render::Context& context)
{
    // Init modules for rendering
    for (auto& module : getModules())
        module.second->drawInit(context);
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Simulation::draw(render::Context& context)
{
    context.setStencilBuffer(getWorldSize().getWidth(), getWorldSize().getHeight());

    // Render modules
    for (auto& module : getModules())
        module.second->draw(context, *this);

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
        const auto hw = getWorldSize().getWidth() * 0.5f;
        const auto hh = getWorldSize().getHeight() * 0.5f;
        const auto sw = getWorldSize().getWidth() / 16.f;
        const auto sh = getWorldSize().getHeight() / 16.f;

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
