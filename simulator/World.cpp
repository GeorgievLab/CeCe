
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
    : m_grid(100)
    , m_flowSpeed(1.f)
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
    if (false)
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

    // Generate diffusion
    {
        const float half = getHeight() / 2.f;

        std::random_device rd;
        std::default_random_engine eng(rd());

        std::bernoulli_distribution d(0.05f);

        // If diffusion should be generated
        //if (d(eng))
        {
            auto& grid = getGrid();

            constexpr int COUNT = 50;
            int step = grid.getHeight() / COUNT;
            int half = grid.getHeight() / 2;

            for (int i = 1; i < COUNT; ++i)
            {
                auto& cell = grid(0, i * step);
                cell.signal = 1.0f;
            }
        }
    }

    // Update all cells
    // Cell update can update cell list
    for (const auto& obj : getObjects())
    {
        assert(obj);
        obj->update(m_timeStep);
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
                ((pos.x >= -wh) && (pos.x <= wh)) &&
                ((pos.y >= -hh) && (pos.y <= hh))
            );
        }), m_objects.end());
    }

    recalcDiffusion(m_timeStep);

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

            // Cell is out of world
            if ((pos.x < 0 || pos.y < 0) ||
                (pos.x >= getWidth() || pos.y >= getHeight()))
                continue;

            auto step_x = getWidth() / m_grid.getWidth();
            auto step_y = getHeight() / m_grid.getHeight();

            // Get grid position
            unsigned int i = pos.x / step_x;
            unsigned int j = pos.y / step_y;

            const GridCell& cell = m_grid(i, j);

            // Use velocity
            ptr->setVelocity(cell.velocity * m_flowSpeed);
        }
    }
}

/* ************************************************************************ */

void World::recalcFlowstreams()
{
    auto& grid = getGrid();
    const auto R = getMainCellRadius();

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
            // Cell center position
            const Vector<float> coord = Vector<float>(i, j) + 0.5f;
            // Real position in the world
            const Vector<float> pos = start + step * coord - m_mainCellPosition;

            // Calculate squared distance from main cell
            const auto distSq = pos.getLengthSquared();

            // Cell is in main cell, ignore
            if (distSq <= R2)
            {
                cell.velocity = Vector<float>{0.f, 0.f};
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
            const float theta = atan2(pos.y, pos.x);

            const Vector<float> u = Vector<float>{
                cosf(theta) * (1 - R2 / distSq),
                -sinf(theta) * (1 + R2 / distSq)
            };

            cell.velocity = u.rotate(theta);
            //cell.velocity = u;
        }
    }

    m_updateGridVelocity = true;
}

/* ************************************************************************ */

void World::recalcDiffusion(units::Duration dt)
{
    static constexpr float PI = 3.14159265359f;

    auto& grid = getGrid();
    const Vector<float> start{-getWidth() / 2.f, -getHeight() / 2.f};
    const Vector<float> step{getWidth() / grid.getWidth(), getHeight() / grid.getHeight()};

    constexpr float D = 0.05f;

    Grid<float> diffGrid(grid.getWidth(), grid.getHeight());
    for (decltype(diffGrid.getWidth()) i = 0; i < diffGrid.getWidth(); ++i)
    {
        for (decltype(diffGrid.getHeight()) j = 0; j < diffGrid.getHeight(); ++j)
        {
            diffGrid(i, j) = -grid(i, j).signal;
        }
    }

    for (decltype(grid.getWidth()) i = 0; i < grid.getWidth(); ++i)
    {
        for (decltype(grid.getHeight()) j = 0; j < grid.getHeight(); ++j)
        {
            auto& cell = grid(i, j);

            // No signal to send
            if (cell.signal == 0)
                continue;

            auto get_cell_pos = [&start, &step](int i, int j) {
                // Transform i, j coordinates to position
                // Cell center position
                const Vector<float> coord = Vector<float>(i, j) + 0.5f;
                // Real position in the world
                return start + step * coord;
            };

            // Get current position
            const Vector<float> pos = get_cell_pos(i, j);

            // Calculate new position
            const Vector<float> new_pos = pos + cell.velocity * dt * getFlowSpeed();

            // Get grid position
            const Vector<float> new_ij_tmp = (new_pos - start) / step;
            const Vector<int> new_ij(new_ij_tmp.x, new_ij_tmp.y);
            const Vector<float> new_pos_center = get_cell_pos(new_ij.x, new_ij.y);
            const Vector<float> offset = new_pos - new_pos_center;

            const float q00 = (Vector<float>( step.x,  step.y) + offset).getLengthSquared();
            const float q01 = (Vector<float>(      0,  step.y) + offset).getLengthSquared();
            const float q02 = (Vector<float>(-step.x,  step.y) + offset).getLengthSquared();
            const float q10 = (Vector<float>( step.x,       0) + offset).getLengthSquared();
            const float q11 = (Vector<float>(      0,       0) + offset).getLengthSquared();
            const float q12 = (Vector<float>(-step.x,       0) + offset).getLengthSquared();
            const float q20 = (Vector<float>( step.x, -step.y) + offset).getLengthSquared();
            const float q21 = (Vector<float>(      0, -step.y) + offset).getLengthSquared();
            const float q22 = (Vector<float>(-step.x, -step.y) + offset).getLengthSquared();

            const float A = 1.f / (4 * PI * D * dt);
            const float c00 = A * exp(-q00 / (4 * D * dt));
            const float c01 = A * exp(-q01 / (4 * D * dt));
            const float c02 = A * exp(-q02 / (4 * D * dt));
            const float c10 = A * exp(-q10 / (4 * D * dt));
            const float c11 = A * exp(-q11 / (4 * D * dt));
            const float c12 = A * exp(-q12 / (4 * D * dt));
            const float c20 = A * exp(-q20 / (4 * D * dt));
            const float c21 = A * exp(-q21 / (4 * D * dt));
            const float c22 = A * exp(-q22 / (4 * D * dt));
            const float cnorm = c00 + c01 + c02 + c10 + c11 + c12 + c20 + c21 + c22;

            // Matrix
            const float M[3][3] = {
                {c00 / cnorm, c01 / cnorm, c20 / cnorm},
                {c10 / cnorm, c11 / cnorm, c21 / cnorm},
                {c20 / cnorm, c21 / cnorm, c22 / cnorm}
            };

            // Out of range
            if ((new_ij.x < 0 || new_ij.y < 0) ||
                (new_ij.x >= grid.getWidth() || new_ij.y >= grid.getHeight()))
                continue;

            const float signal00 = cell.signal * M[0][0];
            const float signal01 = cell.signal * M[0][1];
            const float signal02 = cell.signal * M[0][2];
            const float signal10 = cell.signal * M[1][0];
            const float signal11 = cell.signal * M[1][1];
            const float signal12 = cell.signal * M[1][2];
            const float signal20 = cell.signal * M[2][0];
            const float signal21 = cell.signal * M[2][1];
            const float signal22 = cell.signal * M[2][2];

            // Position of new cell
            if (new_ij.x > 0 && new_ij.y > 0)
                diffGrid(new_ij.x - 1, new_ij.y - 1) += signal00;

            if (new_ij.x > 0)
                diffGrid(new_ij.x - 1, new_ij.y    ) += signal01;

            if (new_ij.x > 0 && (new_ij.y < diffGrid.getHeight() - 1))
                diffGrid(new_ij.x - 1, new_ij.y + 1) += signal02;

            if (new_ij.y > 0)
                diffGrid(new_ij.x,     new_ij.y - 1) += signal10;

            diffGrid(new_ij.x,     new_ij.y    ) += signal11;

            if (new_ij.y < diffGrid.getHeight() - 1)
                diffGrid(new_ij.x,     new_ij.y + 1) += signal12;

            if ((new_ij.x < diffGrid.getWidth() - 1) && (new_ij.y > 0))
                diffGrid(new_ij.x + 1, new_ij.y - 1) += signal20;

            if (new_ij.x < diffGrid.getWidth() - 1)
                diffGrid(new_ij.x + 1, new_ij.y    ) += signal21;

            if ((new_ij.x < diffGrid.getWidth() - 1) && (new_ij.y < diffGrid.getHeight() - 1))
                diffGrid(new_ij.x + 1, new_ij.y + 1) += signal22;
        }
    }

    // Apply changes
    for (decltype(diffGrid.getWidth()) i = 0; i < diffGrid.getWidth(); ++i)
    {
        for (decltype(diffGrid.getHeight()) j = 0; j < diffGrid.getHeight(); ++j)
        {
            const float diff = diffGrid(i, j);

            if (fabs(diff) >= std::numeric_limits<float>::epsilon())
                grid(i, j).signal += diff;
        }
    }

    m_updateGridSignal = true;
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER

void World::render(render::Context& context, RenderFlagsType flags)
{
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

    //if (false)
    {
        if (!m_renderGridSignal)
        {
            std::vector<float> data(m_grid.getWidth() * m_grid.getHeight());
            const auto* grid_data = m_grid.getData();

            for (std::size_t i = 0; i < data.size(); ++i)
                data[i] = grid_data[i].signal;

            m_renderGridSignal.reset(new render::GridValue(
                m_grid.getWidth(), m_grid.getHeight(), data.data()
            ));
        }
        else
        {
            auto rgridw = m_renderGridSignal->getWidth();
            auto rgridh = m_renderGridSignal->getHeight();
            auto gridw = m_grid.getWidth();
            auto gridh = m_grid.getHeight();

            // Resize
            if (!(rgridw == gridw && rgridh == gridh) || m_updateGridSignal)
            {
                std::vector<float> data(m_grid.getWidth() * m_grid.getHeight());
                const auto* grid_data = m_grid.getData();

                for (std::size_t i = 0; i < data.size(); ++i)
                    data[i] = grid_data[i].signal;

                m_renderGridSignal->resize(gridw, gridh, data.data());
                m_updateGridSignal = false;
            }
        }

        assert(m_renderGridSignal);
        m_renderGridSignal->render({getWidth(), getHeight()});
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

            if (!(rgridw == gridw && rgridh == gridh) || m_updateGridVelocity)
            {
                std::vector<Vector<float>> data(m_grid.getWidth() * m_grid.getHeight());
                const auto* grid_data = m_grid.getData();

                for (std::size_t i = 0; i < data.size(); ++i)
                    data[i] = grid_data[i].velocity;

                m_renderGridVelocity->resize(gridw, gridh, data.data());
                m_updateGridVelocity = false;
            }
        }

        // Render grid
        assert(m_renderGridVelocity);
        m_renderGridVelocity->render({getWidth(), getHeight()});
    }

    // Draw main cell
    if (getMainCellRadius())
    {
        const auto& pos = getMainCellPosition();

        context.drawCircle({pos.x, pos.y}, getMainCellRadius(), {0.5f, 0.5f, 0.5f, 0.8f});
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
