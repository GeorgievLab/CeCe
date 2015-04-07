
/* ************************************************************************ */

// Declaration
#include "simulator/DiffusionStreamlinesModule.hpp"

// C++
#include <limits>
#include <algorithm>

// Simulator
#include "core/Matrix.hpp"
#include "simulator/World.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

DiffusionStreamlinesModule::DiffusionStreamlinesModule()
{
    // Nothing to do
}

/* ************************************************************************ */

DiffusionStreamlinesModule::~DiffusionStreamlinesModule()
{
    // Nothing to do
}

/* ************************************************************************ */

void DiffusionStreamlinesModule::update(units::Duration dt, World& world)
{
    // Constants
    constexpr float DIFFUSION_IGNORE = 0.0f;

    StreamlinesModule::update(dt, world);

    auto& signalGrid = DiffusionModule::getGrid();
    auto& velocityGrid = StreamlinesModule::getGrid();

    // Precompute values
    const auto start = world.getStartPosition();
    const auto step = world.calcStep(signalGrid.getSize());

    // Grid for changes
    Grid<float> signalGridNew(signalGrid.getWidth(), signalGrid.getHeight());

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
            Vector<float> dij = velocity * dt * getFlowSpeed() / step;
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
    DiffusionModule::update(dt, world);
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void DiffusionStreamlinesModule::renderInit(render::Context& context)
{
    DiffusionModule::renderInit(context);
    StreamlinesModule::renderInit(context);
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void DiffusionStreamlinesModule::render(render::Context& context, const World& world)
{
    DiffusionModule::render(context, world);
    StreamlinesModule::render(context, world);
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
