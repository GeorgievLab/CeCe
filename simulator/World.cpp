
/* ************************************************************************ */

// Declaration
#include "simulator/World.hpp"

// C++
#include <algorithm>
#include <random>

// Simulator
#include "core/Matrix.hpp"
#include "simulator/StaticObject.hpp"
#include "simulator/DynamicObject.hpp"

// TODO remove
#include "simulator/Cell.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

World::World() noexcept
    : m_timeStep(1.f)
    , m_velocityGrid(10)
    , m_signalGrid(10)
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
            auto& grid = getSignalGrid();

            constexpr int COUNT = 50;
            int step = grid.getHeight() / COUNT;
            int half = grid.getHeight() / 2;

            for (int i = 1; i < COUNT; ++i)
            {
                auto& value = grid(0, i * step);
                value = 1.0f;
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

            // Get grid
            const auto& grid = getVelocityGrid();

            // Get step
            const auto step = getSize() / grid.getSize();

            // Get grid position
            Vector<GridVelocity::SizeType> coord = pos / step;

            // Get velocity
            const auto& velocity = grid[coord];

            // Use velocity
            ptr->setVelocity(velocity * m_flowSpeed);
        }
    }
}

/* ************************************************************************ */

void World::recalcFlowstreams()
{
    auto& grid = getVelocityGrid();
    const auto R = getMainCellRadius();

    // Precompute values
    const auto R2 = R * R;
    const Vector<float> start{-getWidth() / 2.f, -getHeight() / 2.f};
    const Vector<float> step{getWidth() / grid.getWidth(), getHeight() / grid.getHeight()};

    for (decltype(grid.getWidth()) i = 0; i < grid.getWidth(); ++i)
    {
        for (decltype(grid.getHeight()) j = 0; j < grid.getHeight(); ++j)
        {
            auto& velocity = grid(i, j);

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
                velocity = Vector<float>{0.f, 0.f};
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

            velocity = u.rotate(theta);
            //cell.velocity = u;
        }
    }

    m_updateGridVelocity = true;
}

/* ************************************************************************ */

void World::recalcDiffusion(units::Duration dt)
{
    static constexpr float PI = 3.14159265359f;

    auto& grid = getSignalGrid();
    const Vector<float> start{-getWidth() / 2.f, -getHeight() / 2.f};
    const Vector<float> step{getWidth() / grid.getWidth(), getHeight() / grid.getHeight()};

    constexpr float D = 0.5f;

    Grid<float> diffGrid(grid.getWidth(), grid.getHeight());
    for (decltype(diffGrid.getWidth()) i = 0; i < diffGrid.getWidth(); ++i)
    {
        for (decltype(diffGrid.getHeight()) j = 0; j < diffGrid.getHeight(); ++j)
        {
            diffGrid(i, j) = -grid(i, j);
        }
    }

    auto& velocityGrid = getVelocityGrid();

    /// Size of mapping matrix
    static constexpr unsigned MATRIX_SIZE = 3;

    // Offset coefficients for matrix
    static const auto MAPPINGS = Matrix<int, MATRIX_SIZE>::makeDistance();

    for (decltype(grid.getWidth()) i = 0; i < grid.getWidth(); ++i)
    {
        for (decltype(grid.getHeight()) j = 0; j < grid.getHeight(); ++j)
        {
            auto& signal = grid(i, j);

            // No signal to send
            if (signal == 0)
                continue;

            auto& velocity = velocityGrid(i, j);

            // Get current position
            const Vector<float> pos = getPosition(grid, {i, j});

            // Calculate new position
            const Vector<float> new_pos = pos + velocity * dt * getFlowSpeed();

            // Get grid position
            const Vector<float> new_ij_tmp = (new_pos - start) / step;
            const Vector<unsigned> new_ij(new_ij_tmp.x, new_ij_tmp.y);
            const Vector<float> new_pos_center = getPosition(grid, {new_ij.x, new_ij.y});
            const Vector<float> offset = new_pos - new_pos_center;

            /// Generate matrix with coefficients based on distance
            const auto q = Matrix<float, MATRIX_SIZE>::generate([&step, &offset](int i, int j) {
                return (step * MAPPINGS[i][j] + offset).getLengthSquared();
            });

            const float A = 1.f / (4 * PI * D * dt);

            // Create distribution matrix
            const auto M = Matrix<float, MATRIX_SIZE>::generate([A, D, q, dt](int i, int j) {
                return A * exp(-q[i][j] / (4 * D * dt));
            }).normalize();

            // Out of range
            if (!grid.inRange(new_ij.x, new_ij.y))
                continue;

            // Compute signal matrix
            const auto signalMatrix = M * signal;

            // Position of new cell
            if (new_ij.x > 0 && new_ij.y > 0)
                diffGrid(new_ij.x - 1, new_ij.y - 1) += signalMatrix[0][0];

            if (new_ij.x > 0)
                diffGrid(new_ij.x - 1, new_ij.y    ) += signalMatrix[0][1];

            if (new_ij.x > 0 && (new_ij.y < diffGrid.getHeight() - 1))
                diffGrid(new_ij.x - 1, new_ij.y + 1) += signalMatrix[0][2];

            if (new_ij.y > 0)
                diffGrid(new_ij.x,     new_ij.y - 1) += signalMatrix[1][0];

            diffGrid(new_ij.x,     new_ij.y    ) += signalMatrix[1][1];

            if (new_ij.y < diffGrid.getHeight() - 1)
                diffGrid(new_ij.x,     new_ij.y + 1) += signalMatrix[1][2];

            if ((new_ij.x < diffGrid.getWidth() - 1) && (new_ij.y > 0))
                diffGrid(new_ij.x + 1, new_ij.y - 1) += signalMatrix[2][0];

            if (new_ij.x < diffGrid.getWidth() - 1)
                diffGrid(new_ij.x + 1, new_ij.y    ) += signalMatrix[2][1];

            if ((new_ij.x < diffGrid.getWidth() - 1) && (new_ij.y < diffGrid.getHeight() - 1))
                diffGrid(new_ij.x + 1, new_ij.y + 1) += signalMatrix[2][2];
        }
    }

    // Apply changes
    for (decltype(diffGrid.getWidth()) i = 0; i < diffGrid.getWidth(); ++i)
    {
        for (decltype(diffGrid.getHeight()) j = 0; j < diffGrid.getHeight(); ++j)
        {
            const float diff = diffGrid(i, j);

            if (fabs(diff) >= std::numeric_limits<float>::epsilon())
                grid(i, j) += diff;
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
        const auto& grid = getSignalGrid();

        if (!m_renderGridSignal)
        {
            m_renderGridSignal.reset(new render::GridValue(grid.getSize(), grid.getData()));
        }
        else
        {
            const auto& rgridSize = m_renderGridSignal->getSize();
            const auto& gridSize = grid.getSize();

            // Resize
            if (rgridSize != gridSize || m_updateGridSignal)
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
