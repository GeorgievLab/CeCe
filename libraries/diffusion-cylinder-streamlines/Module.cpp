
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <cassert>
#include <limits>
#include <algorithm>

// Simulator
#include "core/Matrix.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion_cylinder_streamlines {

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion-cylinder-streamlines", [&simulation](std::ostream& out, const std::string& name, Clock::duration dt) {
        out << name << ";" << simulation.getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
    });

    assert(m_streamlines);
    assert(m_diffusion);

    auto& signalGrid = m_diffusion->getGrid();
    auto& velocityGrid = m_streamlines->getGrid();

    // Precompute values
    const auto start = simulation.getWorldSize() * 0.5f;
    const auto step = simulation.getWorldSize() / signalGrid.getSize();

    // Grid for changes
    diffusion::SignalGrid signalGridNew(signalGrid.getSize());

    // Sizes must match
    assert(std::distance(signalGrid.begin(), signalGrid.end()) == std::distance(signalGridNew.begin(), signalGridNew.end()));

    // Transform i, j coordinates to position
    auto getPosition = [&start, &step](const Vector<unsigned int>& pos) {
        // Cell center position
        const auto coord = Vector<float>(pos) + 0.5f;
        // Real position
        return start + step * coord;
    };

    for (decltype(signalGrid.getSize().getHeight()) j = 0; j < signalGrid.getSize().getHeight(); ++j)
    {
        for (decltype(signalGrid.getSize().getWidth()) i = 0; i < signalGrid.getSize().getWidth(); ++i)
        {
            const Vector<unsigned> ij(i, j);

            auto& signal = signalGrid[ij];

            // No signal to send
            if (!signal)
                continue;

            // Get velocity
            assert(velocityGrid.inRange(ij));
            auto& velocity = velocityGrid[ij];

            // Calculate coordinate change
            Vector<float> dij = velocity * dt * m_streamlines->getFlowSpeed() / step;
            dij.x() = std::abs(dij.getX());
            dij.y() = std::abs(dij.getY());

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
                    Vector<unsigned int> ab(
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
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    assert(m_streamlines);
    assert(m_diffusion);

    m_diffusion->draw(context, simulation);
    m_streamlines->draw(context, simulation);
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
