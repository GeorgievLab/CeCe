
/* ************************************************************************ */

// Declaration
#include "simulator/Simulation.hpp"

// C++
#include <algorithm>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Simulation::Simulation() noexcept
#ifdef ENABLE_PHYSICS
 : m_world{b2Vec2{0.0f, 0.0f}}
#endif
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
    for (std::size_t i = 0; i < getObjects().size(); ++i)
    {
        auto& obj = getObjects()[i];
        assert(obj);
        obj->update(dt);
    }

    // Remove objects that are outside world.
    {
        const auto hh = getWorldSize() * 0.5f;

        // Kill objects that are outside world
        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [&hh](const ObjectContainer::value_type& obj) {
            // Ignore static objects
            if (obj->getType() == Object::Type::Static)
                return false;

            // Get object position
            const auto& pos = obj->getPosition();

            // TODO: optimize
            return not (
                ((pos.getX() >= -hh.getX()) && (pos.getX() <= hh.getX())) &&
                ((pos.getY() >= -hh.getY()) && (pos.getY() <= hh.getY()))
            );
        }), m_objects.end());

        // Remove objects from drawInit
        m_drawInitList.erase(std::remove_if(m_drawInitList.begin(), m_drawInitList.end(), [this](Object* ptr) {
            return std::find_if(m_objects.begin(), m_objects.end(), [ptr](const ObjectContainer::value_type& obj) {
                return obj.get() == ptr;
            }) == m_objects.end();
        }), m_drawInitList.end());
    }

#ifdef ENABLE_PHYSICS
    m_world.Step(dt, 5, 10);
#endif
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
