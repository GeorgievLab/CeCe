
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <limits>
#include <algorithm>

// Simulator
#include "core/Matrix.hpp"
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion_streamlines {

/* ************************************************************************ */

Module::~Module()
{
    // Nothing to do
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::World& world)
{
    // Constants
    constexpr float DIFFUSION_IGNORE = 0.0f;

    m_streamlines.update(dt, world);

    auto& signalGrid = m_diffusion.getGrid();
    auto& velocityGrid = m_streamlines.getGrid();

    // Precompute values
    const auto start = world.getStartPosition();
    const auto step = world.calcStep(signalGrid.getSize());

    // Grid for changes
    diffusion::SignalGrid signalGridNew(signalGrid.getWidth(), signalGrid.getHeight());

    // Sizes must match
    assert(std::distance(signalGrid.begin(), signalGrid.end()) == std::distance(signalGridNew.begin(), signalGridNew.end()));

    // Transform i, j coordinates to position
    auto getPosition = [&start, &step](const Vector<unsigned int>& pos) {
        // Cell center position
        const Vector<float> coord = Vector<float>(pos) + 0.5f;
        // Real position
        return start + step * coord;
    };

    for (decltype(signalGrid.getHeight()) j = 0; j < signalGrid.getHeight(); ++j)
    {
        for (decltype(signalGrid.getWidth()) i = 0; i < signalGrid.getWidth(); ++i)
        {
            const Vector<unsigned> ij(i, j);

            auto& signal = signalGrid[ij];

            // No signal to send
            if (signal <= DIFFUSION_IGNORE)
                continue;

            // Get velocity
            assert(velocityGrid.inRange(ij));
            auto& velocity = velocityGrid[ij];

            // Calculate coordinate change
            Vector<float> dij = velocity * dt * m_streamlines.getFlowSpeed() / step;
            dij.getX() = std::abs(dij.getX());
            dij.getY() = std::abs(dij.getY());

            Matrix<float, 2> tmp;
            int offset = 0;

            if (velocity.getY() < 0)
            {
                tmp = Matrix<float, 2>{{
                    {(1 - dij.getX()) *      dij.getY() , dij.getX() *      dij.getY() },
                    {(1 - dij.getX()) * (1 - dij.getY()), dij.getX() * (1 - dij.getY())}
                }};
                offset = 1;
            }
            else
            {
                tmp = Matrix<float, 2>{{
                    {(1 - dij.getX()) * (1 - dij.getY()), dij.getX() * (1 - dij.getY())},
                    {(1 - dij.getX()) *      dij.getY() , dij.getX() *      dij.getY() }
                }};
                offset = 0;
            }

            // TODO: optimize
            for (unsigned a = 0; a < 2; ++a)
            {
                for (unsigned b = 0; b < 2; ++b)
                {
                    Vector<int> ab(
                        ij.getX() + b,
                        ij.getY() - offset + a
                    );

                    if (signalGridNew.inRange(ab))
                        signalGridNew[ab] += signal * tmp[a][b];
                }
            }
        }
    }

    // Replace the old grid with the new one
    signalGrid = std::move(signalGridNew);

    // Update diffusion
    m_diffusion.update(dt, world);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::drawInit(render::Context& context)
{
    m_diffusion.drawInit(context);
    m_streamlines.drawInit(context);
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::World& world)
{
    m_diffusion.draw(context, world);
    m_streamlines.draw(context, world);
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
