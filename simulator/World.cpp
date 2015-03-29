
/* ************************************************************************ */

// Declaration
#include "simulator/World.hpp"

// C++
#include <algorithm>
#include <random>

// Simulator
#include "simulator/StaticObject.hpp"
#include "simulator/DynamicObject.hpp"

// TODO remove
#include "simulator/Cell.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World() noexcept
    : m_grid(2)
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

void World::update() noexcept
{
    // Increase step number
    m_stepNumber++;

    // Generate cells
    {
        const float half = getHeight() / 2.f;

        std::random_device rd;
        std::default_random_engine eng(rd());

        std::bernoulli_distribution d(0.5);

        // If cell should be generated
        if (d(eng))
        {
            std::uniform_real_distribution<float> dist(-half, half);
            float y = dist(eng);

            // Create cell
            Cell* cell = createObject<Cell>();
            cell->setVolume(units::um3(200));
            cell->setVelocity({10, 0});
            cell->setPosition({-getWidth() / 2.f, y});
        }
    }

    // Update all cells
    // Cell update can update cell list
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->update(m_timeStep);
    }

    // Apply streamlines
    {
        const Vector<float> start{
            -getWidth() / 2.f,
            -getHeight() / 2.f
        };

        for (auto& obj : getObjects())
        {
            auto ptr = dynamic_cast<DynamicObject*>(obj.get());
            if (!ptr)
                continue;

            auto pos = ptr->getPosition();
            pos.x -= start.x;
            pos.y -= start.y;

            auto step_x = getWidth() / m_grid.getWidth();
            auto step_y = getHeight() / m_grid.getHeight();

            // Get grid position
            unsigned int i = pos.x / step_x;
            unsigned int j = pos.y / step_y;

            const GridCell& cell = m_grid(i, j);

            // Use velocity
            ptr->setVelocity(cell.velocity);
        }
    }

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
                ((pos.x >= -wh) && (pos.x <= wh)) &&
                ((pos.y >= -hh) && (pos.y <= hh))
            );
        }), m_objects.end());
    }
}

/* ************************************************************************ */

void World::recalcFlowstreams()
{
    auto& grid = getGrid();
    const auto R = getMainCellRadius();
    const auto U = 10000;

    // Precompute values
    const auto R2 = R * R;
    const Vector<float> start{-getWidth() / 2.f, -getHeight() / 2.f};
    const Vector<float> step{getWidth() / grid.getWidth(), getHeight() / grid.getHeight()};

    for (decltype(grid.getWidth()) i = 0; i < grid.getWidth(); ++i)
    {
        for (decltype(grid.getHeight()) j = 0; j < grid.getHeight(); ++j)
        {
            auto& cell = grid(i, j);

            // Transform i, j coordinates to position
            const float x = start.x + i * step.x + step.x / 2.f;
            const float y = start.y + j * step.y + step.y / 2.f;

            const auto r2 = x * x + y * y;
            //auto theta = atan2(i2, j2);
            //auto ur = 1 * cos(theta) * (1 - R * R / (r * r));
            //auto ut = -1 * sin(theta) * (1 + R * R / (r * r));
            //auto u = ur * cos(theta) - ut * sin(theta);
            //auto v = ur * sin(theta) + ut * cos(theta);

            if (r2 <= R2)
                continue;

            // Precompute values
            const float r4 = r2 * r2;
            const float x2 = x * x;
            const float xy = x * y;

            // COPYRIGHT: Hynek magic
            cell.velocity.x = U * (1 + R2 / r2 - 2 * (x2 * R2) / r4);
            cell.velocity.y = U * -2 * (R2 * xy) / r4;
        }
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void World::render(render::Context& context, RenderFlagsType flags)
{

    {
        auto rgridw = m_renderGridSignal.getWidth();
        auto rgridh = m_renderGridSignal.getHeight();
        auto gridw = m_grid.getWidth();
        auto gridh = m_grid.getHeight();

        // Resize
        if (!(rgridw == gridw && rgridh == gridh))
        {
            std::vector<unsigned char> data(m_grid.getWidth() * m_grid.getHeight());
            //const auto* grid_data = m_grid.getData();

            std::random_device rd;
            std::default_random_engine e1(rd());
            std::uniform_int_distribution<int> uniform_dist(0, 255);

            for (std::size_t i = 0; i < data.size(); ++i)
                //data[i] = grid_data[i].signal;
                data[i] = uniform_dist(e1);

            m_renderGridSignal.resize(gridw, gridh, data.data());
        }

        m_renderGridSignal.render({getWidth(), getHeight()});
    }

    if (flags & RENDER_GRID)
    {
        if (!m_renderGrid)
        {
            m_renderGrid.reset(new render::Grid{m_grid.getWidth(), m_grid.getHeight()});
        }
        else
        {
            auto rgridw = m_renderGrid->getWidth();
            auto rgridh = m_renderGrid->getHeight();
            auto gridw = m_grid.getWidth();
            auto gridh = m_grid.getHeight();

            if (!(rgridw == gridw && rgridh == gridh))
                m_renderGrid->resize(gridw, gridh);
        }

        // Render grid
        assert(m_renderGrid);
        m_renderGrid->render({getWidth(), getHeight()}, {0.9f, 0.5f, 0.5f, 0.2f});
    }

    if (flags & RENDER_VELOCITY)
    {
        if (!m_renderGridVelocity)
        {
            std::vector<Vector<float>> data(m_grid.getWidth() * m_grid.getHeight());
            const auto* grid_data = m_grid.getData();

            for (std::size_t i = 0; i < data.size(); ++i)
                data[i] = grid_data[i].velocity;

            m_renderGridVelocity.reset(new render::GridVector{
                m_grid.getWidth(), m_grid.getHeight(), data.data()
            });
        }
        else
        {
            auto rgridw = m_renderGridVelocity->getWidth();
            auto rgridh = m_renderGridVelocity->getHeight();
            auto gridw = m_grid.getWidth();
            auto gridh = m_grid.getHeight();

            if (!(rgridw == gridw && rgridh == gridh))
            {
                std::vector<Vector<float>> data(m_grid.getWidth() * m_grid.getHeight());
                const auto* grid_data = m_grid.getData();

                for (std::size_t i = 0; i < data.size(); ++i)
                    data[i] = grid_data[i].velocity;

                m_renderGridVelocity->resize(gridw, gridh, data.data());
            }
        }

        // Render grid
        assert(m_renderGridVelocity);
        m_renderGridVelocity->render({getWidth(), getHeight()});
    }

    // Draw main cell
    if (getMainCellRadius())
    {
        context.drawCircle({0, 0}, getMainCellRadius(), {0, 0, 0, 1});
    }

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
