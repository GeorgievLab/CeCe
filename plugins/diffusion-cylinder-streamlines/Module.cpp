/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <cassert>

// Simulator
#include "core/StaticMatrix.hpp"
#include "core/TimeMeasurement.hpp"
#include "core/VectorRange.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion_cylinder_streamlines {

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion-cylinder-streamlines", simulator::TimeMeasurementIterationOutput(simulation));

    assert(m_streamlines);
    assert(m_diffusion);

    const auto signalGridSize = m_diffusion->getGridSize();
    auto& velocityGrid = m_streamlines->getGrid();

    // Precompute values
    const auto step = simulation.getWorldSize() / signalGridSize;

    // Foreach signals
    for (auto id : m_diffusion->getSignalIds())
    {
        m_diffusion->clearBack(id);

        // Foreach all combination in range [0, 0] - signalGrid.getSize()
        for (auto&& c : range(signalGridSize))
        {
            // Get current signal
            auto& signal = m_diffusion->getSignal(id, c);

            // No signal to send
            if (!signal)
                continue;

            // Get velocity
            assert(velocityGrid.inRange(c));
            auto& velocity = velocityGrid[c];

            // Calculate coordinate change
            Vector<float> dij = velocity * dt * m_streamlines->getFlowSpeed() / step;
            dij.x() = std::abs(dij.getX());
            dij.y() = std::abs(dij.getY());

            StaticMatrix<float, 2> tmp;
            int offset = 0;

            if (velocity.getY() < units::Velocity(0))
            {
                tmp = StaticMatrix<float, 2>{{
                    {(1 - dij.getX()) *      dij.getY() , dij.getX() *      dij.getY() },
                    {(1 - dij.getX()) * (1 - dij.getY()), dij.getX() * (1 - dij.getY())}
                }};
                offset = 1;
            }
            else
            {
                tmp = StaticMatrix<float, 2>{{
                    {(1 - dij.getX()) * (1 - dij.getY()), dij.getX() * (1 - dij.getY())},
                    {(1 - dij.getX()) *      dij.getY() , dij.getX() *      dij.getY() }
                }};
                offset = 0;
            }

            // Apply matrix
            for (auto&& ab : range(Vector<unsigned>(2)))
            {
                const auto ab2 = c + ab - Vector<unsigned>(0, offset);

                // Update signal
                if (m_diffusion->inRange(ab2))
                    m_diffusion->getSignalBack(id, ab2) += signal * tmp[ab];
            }
        }

        // Swap buffers
        m_diffusion->swap(id);
    }
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
