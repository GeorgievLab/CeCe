
// Declaration
#include "simulator/DiffusionModule.hpp"

// C++
#include <limits>
#include <algorithm>
#include <functional>
#include <random>

// Simulator
#include "core/Matrix.hpp"
#include "core/constants.hpp"
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

/// GENERATION
    if (true)
    {
        constexpr float SOURCE_STRENGTH = 1000.f;

        const float grid_half = m_grid.getHeight() / 2;
        const int off = m_grid.getHeight() * 0.1f;

        for (int i = -1; i < 1; ++i)
            m_grid(0, grid_half + (i + 0.5) * off) += SOURCE_STRENGTH * dt;
    }
/// END

    // Size of mapping matrix
    constexpr unsigned OFFSET = 1;
    constexpr unsigned MATRIX_SIZE = 2 * OFFSET + 1;
    constexpr float DIFFUSION_IGNORE = 0.0f;

    // Diffusion constant
    constexpr float D = 0.1f;

    // Precompute values
    const auto start = world.getStartPosition();
    const auto step = world.calcStep(m_grid.getSize());

    //const float A = 1.f / (4 * constants::PI * D * dt);
    const float A = 1.f / (constants::PI * dt);

    // Offset coefficients for matrix
    static const auto DISTANCES = Matrix<int, MATRIX_SIZE>::makeDistances();

    // Create grid with modified values
    Grid<float> gridNew(m_grid.getWidth(), m_grid.getHeight());

    // Sizes must match
    assert(std::distance(m_grid.begin(), m_grid.end()) == std::distance(gridNew.begin(), gridNew.end()));

    // Generate matrix with coefficients based on distance
    // TODO: precompute matrix
    const auto q = Matrix<float, MATRIX_SIZE>::generate([&step](int i, int j) {
        return (step * DISTANCES[i][j]).getLengthSquared();
    });

    // Create distribution matrix
    const auto M = Matrix<float, MATRIX_SIZE>::generate([A, D, q, dt](int i, int j) {
        //return A * exp(-q[i][j] / (4 * D * dt));
        return A * exp(-q[i][j] / dt);
    }).normalize();

    // Recaulculate diffusion
    for (decltype(m_grid.getHeight()) j = 0; j < m_grid.getHeight(); ++j)
    {
        for (decltype(m_grid.getWidth()) i = 0; i < m_grid.getWidth(); ++i)
        {
            const Vector<GridType::SizeType> ij(i, j);

            auto& signal = m_grid[ij];

            // No signal to send
            if (signal <= DIFFUSION_IGNORE)
                continue;

            // Compute signal matrix
            const auto signalMatrix = M * signal;

            // TODO: optimize
            for (unsigned a = 0; a < MATRIX_SIZE; ++a)
            {
                for (unsigned b = 0; b < MATRIX_SIZE; ++b)
                {
                    Vector<int> ab(
                        ij.getX() - OFFSET + b,
                        ij.getY() - OFFSET + a
                    );

                    if (gridNew.inRange(ab))
                        gridNew[ab] += signalMatrix[a][b];
                }
            }
        }
    }

    // Replace the old grid with the new one
    m_grid = std::move(gridNew);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void DiffusionModule::renderInit(render::Context& context)
{
    m_renderObject.init(m_grid.getSize(), m_grid.getData());
    m_renderGridObject.init();
    m_renderGridObject.resize(m_grid.getSize());
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void DiffusionModule::render(render::Context& context, const World& world)
{
    m_renderObject.update(m_grid.getData());
    m_renderObject.render(world.getSize());
    m_renderGridObject.render(world.getSize(), {1, 0, 0, 1});
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
