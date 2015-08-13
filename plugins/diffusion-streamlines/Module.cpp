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

// Plugin
#include "plugins/streamlines/LatticeData.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion_streamlines {

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion-streamlines", simulator::TimeMeasurementIterationOutput(simulation));

    assert(m_streamlines);
    assert(m_diffusion);

    const auto signalGridSize = m_diffusion->getGridSize();
    auto& velocityGrid = m_streamlines->getLattice();

    // Physical size of one lattice cell
    const auto dl = simulation.getWorldSize() / velocityGrid.getSize();
    // Calculate maximum flow velocity
    const VelocityVector v_max = plugin::streamlines::LatticeData::MAX_SPEED * dl / dt;

    // Precompute values
    const auto step = simulation.getWorldSize() / m_diffusion->getGridSize();

    // Scale grids to [0, 1]
    const auto signalScale = 1.f / signalGridSize;
    const auto velocityScale = 1.f / velocityGrid.getSize();
    const auto scale = signalScale / velocityScale;

#if THREAD_SAFE
    // Lock access to both modules
    // No deadlock posibility
    MutexGuard guardDiffusion(m_diffusion->getMutex());
    //MutexGuard guardStreamlines(m_streamlines->getMutex());
#endif

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

            // Calculate velocity scale
            const auto vc = Vector<unsigned>(c * scale);

            // Get velocity
            assert(velocityGrid.inRange(vc));
            const auto& velocity = velocityGrid[vc].calcVelocity() * v_max;

            // Calculate coordinate change
            Vector<float> dij = velocity * dt / step;
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

}
}

/* ************************************************************************ */
