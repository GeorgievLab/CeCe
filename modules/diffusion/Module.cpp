
// Declaration
#include "Module.hpp"

// C++
#include <limits>
#include <functional>
#include <random>

// Simulator
#include "core/Matrix.hpp"
#include "core/constants.hpp"
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::World& world)
{
    // Size of mapping matrix
    constexpr unsigned OFFSET = 1;
    constexpr unsigned MATRIX_SIZE = 2 * OFFSET + 1;

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
    decltype(m_grid) gridNew(m_grid.getWidth(), m_grid.getHeight());

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

    /*
     * Algorithm:
     * Foreach whole grid in single for loop - grid storage is 1D array.
     * This solution remove requirements for computation array offset each time
     * for reading and writing.
     * Diffusion mapping matrix contains offsets from current position to
     * the coresponding positions. Mapping matrix can be constructed as:
     * to right: +1; to left: -1; up: -width; down: +width. For 3x3 it looks:
     *   | -width - 1, -width, -width + 1 |
     *   |         -1       0,         +1 |
     *   | +width - 1, +width, +width + 1 |
     *
     * This solutions improve range checking as well. Because range is just
     * [0, width * height), only one range check is required.
     */

    const int width = m_grid.getWidth();
    const int height = m_grid.getHeight();

    const Matrix<int, 3> MAPPING_MATRIX{{
        {-width - 1, -width, -width + 1},
        {       - 1,      0,        + 1},
        {+width - 1, +width, +width + 1}
    }};

    // Current column
    int column = 0;
    int row = 0;

    // Foreach grid cells
    auto gridPtr = m_grid.cbegin();
    auto gridEndPtr = m_grid.cend();
    auto gridNewPtr = gridNew.begin();

    // Grid sizes are same, so we don't have to check both ranges
    for (; gridPtr != gridEndPtr; ++gridPtr, ++gridNewPtr)
    {
        // Get current signal
        const auto signal = *gridPtr;

        // No signal to send
        if (signal)
        {
            // TODO: optimize
            for (unsigned a = row < OFFSET ? OFFSET - row : 0;
                 a < ((height - row) <= OFFSET ? MATRIX_SIZE - (height - row) : MATRIX_SIZE);
                 ++a)
            {
                for (unsigned b = column < OFFSET ? OFFSET - column : 0;
                     b < ((width - column) <= OFFSET ? MATRIX_SIZE - (width - column) : MATRIX_SIZE);
                     ++b)
                {
                    auto ptr = gridNewPtr + MAPPING_MATRIX[a][b];

                    assert(ptr >= gridNew.begin() && ptr < gridNew.end());
                    *ptr += signal * M[a][b];
                }
            }
        }

        if (++column == width)
        {
            column = 0;
            ++row;
        }

    }

    {
#ifdef THREAD_SAFE
        std::lock_guard<std::mutex> lock(m_mutex);
#endif

        // Replace the old grid with the new one
        m_grid = std::move(gridNew);
    }
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::drawInit(render::Context& context)
{
#ifdef THREAD_SAFE
    std::lock_guard<std::mutex> lock(m_mutex);
#endif

    getDrawable().init(m_grid.getSize(), m_grid.getData());
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::World& world)
{
    {
#ifdef THREAD_SAFE
        std::lock_guard<std::mutex> lock(m_mutex);
#endif
        getDrawable().update(m_grid.getData());
    }

    getDrawable().draw(world.getSize());
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
