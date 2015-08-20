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
#include "core/Real.hpp"
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
    const VelocityVector vMax = m_streamlines->calculateMaxVelocity(dl);

    // Precompute values
    const auto step = simulation.getWorldSize() / signalGridSize;

    // Scale grids to [0, 1]
    const auto signalScale = 1.f / signalGridSize;
    const auto velocityScale = 1.f / velocityGrid.getSize();
    const auto scale = signalScale / velocityScale;

    // Foreach signals
    for (auto id : m_diffusion->getSignalIds())
    {
        m_diffusion->clearBack(id);

        // Foreach all combination in range [0, 0] - signalGrid.getSize()
        for (auto&& c : range(signalGridSize))
        {
            // Get current signal
            const auto signal = m_diffusion->getSignal(id, c);

            // No signal to send
            if (!signal)
                continue;

            // Calculate velocity scale
            const auto vc = Vector<unsigned>(c * scale);

            // Get velocity
            assert(velocityGrid.inRange(vc));
            const auto& velocity = velocityGrid[vc].calcVelocity() * vMax;

            // Calculate coordinate change
            Vector<RealType> dij = velocity * dt / step;
            dij.x() = std::abs(dij.getX());
            dij.y() = std::abs(dij.getY());

            // Integer value of coordinate change
            const auto iij = Vector<RealType>{std::floor(dij.getX()), std::floor(dij.getY())};
            const auto dij2 = dij - iij;

            StaticMatrix<RealType, 2> tmp;
            int offset = 0;

            if (velocity.getY() < Zero)
            {
                tmp = StaticMatrix<RealType, 2>{{
                    {(1 - dij2.getX()) *      dij2.getY() , dij2.getX() *      dij2.getY() },
                    {(1 - dij2.getX()) * (1 - dij2.getY()), dij2.getX() * (1 - dij2.getY())}
                }};
                offset = 1;
            }
            else
            {
                tmp = StaticMatrix<RealType, 2>{{
                    {(1 - dij2.getX()) * (1 - dij2.getY()), dij2.getX() * (1 - dij2.getY())},
                    {(1 - dij2.getX()) *      dij2.getY() , dij2.getX() *      dij2.getY() }
                }};
                offset = 0;
            }

            // Apply matrix
            for (auto&& ab : range(Vector<unsigned>(2)))
            {
                const auto ab2 = c + ab + Vector<unsigned>(iij) - Vector<unsigned>(0, offset);

                // Update signal
                if (m_diffusion->inRange(ab2))
                {
                    m_diffusion->getSignalBack(id, ab2) += signal * tmp[ab];
                    assert(m_diffusion->getSignalBack(id, ab2) >= 0);
                }
            }
        }
    }

    {
#if THREAD_SAFE
        // Lock diffusion
        MutexGuard guardDiffusion(m_diffusion->getMutex());
#endif

        // Swap buffers
        for (auto id : m_diffusion->getSignalIds())
            m_diffusion->swap(id);
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
