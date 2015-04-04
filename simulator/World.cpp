
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
            auto ptr = dynamic_cast<StaticObject*>(obj.get());

            if (!ptr)
                return false;

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
void World::renderInit(render::Context& context)
{
    // Prepare objects for rendering
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->renderInit(context);
    }
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void World::render(render::Context& context, RenderFlagsType flags)
{
    // Prepare objects for rendering
    {
        for (auto ptr : m_renderInitList)
        {
            assert(ptr);
            ptr->renderInit(context);
        }

        m_renderInitList.clear();
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
/*
    //if (false)
    {
        const auto& grid = getSignalGrid();

        if (!m_renderGridSignal)
        {
            m_renderGridSignal.reset(new render::GridValue(grid.getSize(), grid.getData()));
        }
        else if (m_updateGridSignal)
        {
            m_renderGridSignal->update(grid.getData());
            m_updateGridSignal = false;
        }
        else
        {
            const auto& rgridSize = m_renderGridSignal->getSize();
            const auto& gridSize = grid.getSize();

            // Resize
            if (rgridSize != gridSize)
            {
                m_renderGridSignal->resize(gridSize, grid.getData());
                m_updateGridSignal = false;
            }
        }

        assert(m_renderGridSignal);
        m_renderGridSignal->render(getSize());
    }

    if (flags & RENDER_GRID)
    {
        const auto& grid = getVelocityGrid();

        if (!m_renderGrid)
        {
            m_renderGrid.reset(new render::Grid{grid.getWidth(), grid.getHeight()});
        }
        else
        {
            auto rgridw = m_renderGrid->getWidth();
            auto rgridh = m_renderGrid->getHeight();
            auto gridw = grid.getWidth();
            auto gridh = grid.getHeight();

            if (!(rgridw == gridw && rgridh == gridh))
                m_renderGrid->resize(gridw, gridh);
        }

        // Render grid
        assert(m_renderGrid);
        m_renderGrid->render({getWidth(), getHeight()}, {0.9f, 0.5f, 0.5f, 0.2f});
    }

    if (flags & RENDER_VELOCITY)
    {
        const auto& grid = getVelocityGrid();

        if (!m_renderGridVelocity)
        {
            m_renderGridVelocity.reset(new render::GridVector{
                grid.getWidth(), grid.getHeight(), grid.getData()
            });
        }
        else
        {
            auto rgridw = m_renderGridVelocity->getWidth();
            auto rgridh = m_renderGridVelocity->getHeight();
            auto gridw = grid.getWidth();
            auto gridh = grid.getHeight();

            if (!(rgridw == gridw && rgridh == gridh) || m_updateGridVelocity)
            {
                m_renderGridVelocity->resize(gridw, gridh, grid.getData());
                m_updateGridVelocity = false;
            }
        }

        // Render grid
        assert(m_renderGridVelocity);
        m_renderGridVelocity->render({getWidth(), getHeight()});
    }
*/
    // Draw objects
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->render(context);
    }
}

#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
