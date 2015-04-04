
// Declaration
#include "simulator/DiffusionModule.hpp"

// C++
#include <limits>
#include <algorithm>
#include <functional>
#include <random>

// Simulator
#include "core/Matrix.hpp"
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

DiffusionModule::DiffusionModule()
    : m_grid(500)
{
    // Nothing to do
}

/* ************************************************************************ */

DiffusionModule::~DiffusionModule()
{
    // Nothing to do
}

/* ************************************************************************ */

void DiffusionModule::update(units::Duration dt, World& world)
{

    // Generate diffusion
    if (false)
    {
        auto& grid = getGrid();

        std::random_device rd;
        std::default_random_engine eng(rd());

        //std::bernoulli_distribution is(0.01f);

        //if (is(eng))
        {
            std::uniform_int_distribution<int> x_gen(0, grid.getWidth() - 1);
            std::uniform_int_distribution<int> y_gen(0, grid.getHeight() - 1);

            std::uniform_int_distribution<int> count_gen(0, 10);
            int count = count_gen(eng);

            std::uniform_real_distribution<float> val_gen(0.2f, 1.f);

            for (int i = 0; i < count; ++i)
            {
                grid(x_gen(eng), y_gen(eng)) += val_gen(eng);
            }
        }
    }
    else if (true)
    {
        std::random_device rd;
        std::default_random_engine eng(rd());

        std::bernoulli_distribution d(0.05f);

        // If diffusion should be generated
        //if (d(eng))
        {
            auto& grid = getGrid();

            constexpr int COUNT = 20;
            int step = grid.getHeight() / COUNT;
            int half = grid.getHeight() / 2;

            for (int i = 1; i < COUNT; ++i)
            {
                auto& value = grid(0, i * step);
                value = 2.f;
            }
        }
    }


    static constexpr float PI = 3.14159265359f;
    /// Size of mapping matrix
    static constexpr unsigned MATRIX_SIZE = 3;
    constexpr float D = 5.f;

    auto& grid = getGrid();

    // Precompute values
    const auto start = world.getStartPosition();
    const auto step = world.calcStep(grid.getSize());

    const float A = 1.f / (4 * PI * D * dt);

    // Offset coefficients for matrix
    static const auto MAPPINGS = Matrix<int, MATRIX_SIZE>::makeDistance();

    // Create grid for changes
    Grid<float> diffGrid(grid.getWidth(), grid.getHeight());

    // Sizes must match
    assert(std::distance(grid.begin(), grid.end()) == std::distance(diffGrid.begin(), diffGrid.end()));

    // Generate initial diff grid values (remove previous values)
    std::transform(grid.begin(), grid.end(), diffGrid.begin(), std::negate<float>());

    // Transform i, j coordinates to position
    // TODO: to function
    auto getPosition = [&start, &step](const Vector<unsigned int>& pos) {
        // Cell center position
        const Vector<float> coord = Vector<float>(pos) + 0.5f;
        // Real position
        return start + step * coord;
    };

    // Generate matrix with coefficients based on distance
    // TODO: precompute matrix
    const auto q = Matrix<float, MATRIX_SIZE>::generate([&step](int i, int j) {
        return (step * MAPPINGS[i][j]).getLengthSquared();
    });

    // Create distribution matrix
    const auto M = Matrix<float, MATRIX_SIZE>::generate([A, D, q, dt](int i, int j) {
        return A * exp(-q[i][j] / (4 * D * dt));
    }).normalize();

    // Diffusion
    for (decltype(grid.getHeight()) j = 0; j < grid.getHeight(); ++j)
    {
        for (decltype(grid.getWidth()) i = 0; i < grid.getWidth(); ++i)
        {
            const Vector<GridType::SizeType> ij(i, j);

            auto& signal = grid[ij];

            // No signal to send
            if (signal == 0)
                continue;

            // Get current position
            const Vector<float> pos = getPosition(ij);

            // Compute signal matrix
            const auto signalMatrix = M * signal;

            constexpr unsigned int MATRIX_HALF = MATRIX_SIZE / 2;

            // TODO: optimize
            for (unsigned a = 0; a < MATRIX_SIZE; ++a)
            {
                for (unsigned b = 0; b < MATRIX_SIZE; ++b)
                {
                    Vector<int> ab(
                        ij.getX() - MATRIX_HALF + b,
                        ij.getY() - MATRIX_HALF + a
                    );

                    if (diffGrid.inRange(ab))
                        diffGrid[ab] += signalMatrix[a][b];
                }
            }
        }
    }

    // Add changes to the main grid
    std::transform(grid.begin(), grid.end(), diffGrid.begin(), grid.begin(), std::plus<float>());
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void DiffusionModule::renderInit(render::Context& context)
{
    m_renderObject.init(m_grid.getSize(), m_grid.getData());
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void DiffusionModule::render(render::Context& context, const World& world)
{
    m_renderObject.update(m_grid.getData());
    m_renderObject.render(world.getSize());
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
