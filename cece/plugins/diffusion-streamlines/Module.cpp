/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/StaticMatrix.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/simulator/TimeMeasurement.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugins
#include "cece/plugins/diffusion/Module.hpp"
#include "cece/plugins/streamlines/Module.hpp"
#include "cece/plugins/streamlines/Lattice.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion_streamlines {

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    m_streamlines = getSimulation().useModule<plugin::streamlines::Module>("streamlines");
    m_diffusion = getSimulation().useModule<plugin::diffusion::Module>("diffusion");
}

/* ************************************************************************ */

void Module::update()
{
    auto _ = measure_time("diffusion-streamlines", simulator::TimeMeasurement(getSimulation()));

    Assert(m_streamlines);
    Assert(m_diffusion);

    const auto signalGridSize = m_diffusion->getGridSize();
    auto& velocityGrid = m_streamlines->getLattice();

    // Precompute values
    const auto step = getSimulation().getWorldSize() / signalGridSize;

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
            const auto& velocity = m_streamlines->convertVelocity(velocityGrid[vc].computeVelocity());


            // TODO: Completely redesign

            // Calculate coordinate change
            Vector<RealType> dij = velocity * getSimulation().getTimeStep() / step;
            dij.x() = std::abs(dij.getX());
            dij.y() = std::abs(dij.getY());

            // Integer value of coordinate change
            const auto iij = Vector<RealType>{std::floor(dij.getX()), std::floor(dij.getY())};
            const auto dij2 = dij - iij;

            StaticMatrix<RealType, 2> tmp;
            Vector<unsigned> offset = Zero;

            if (velocity.getY() < Zero)
            {
                offset.y() = 1;

                if (velocity.getX() < Zero)
                {
                    offset.x() = 1;
                    tmp = StaticMatrix<RealType, 2>{{
                        {dij2.getX() *      dij2.getY() , (1 - dij2.getX()) *      dij2.getY() },
                        {dij2.getX() * (1 - dij2.getY()), (1 - dij2.getX()) * (1 - dij2.getY())}
                    }};
                }
                else
                {
                    tmp = StaticMatrix<RealType, 2>{{
                        {(1 - dij2.getX()) *      dij2.getY() , dij2.getX() *      dij2.getY() },
                        {(1 - dij2.getX()) * (1 - dij2.getY()), dij2.getX() * (1 - dij2.getY())}
                    }};
                }
            }
            else
            {
                if (velocity.getX() < Zero)
                {
                    offset.x() = 1;
                    tmp = StaticMatrix<RealType, 2>{{
                        {dij2.getX() * (1 - dij2.getY()), (1 - dij2.getX()) * (1 - dij2.getY())},
                        {dij2.getX() *      dij2.getY() , (1 - dij2.getX()) *      dij2.getY() }
                    }};
                }
                else
                {
                    tmp = StaticMatrix<RealType, 2>{{
                        {(1 - dij2.getX()) * (1 - dij2.getY()), dij2.getX() * (1 - dij2.getY())},
                        {(1 - dij2.getX()) *      dij2.getY() , dij2.getX() *      dij2.getY() }
                    }};
                }
            }

            // Apply matrix
            for (auto&& ab : range(Vector<unsigned>::createSingle(2)))
            {
                const auto ab2 = c + ab + Vector<unsigned>(iij) - offset;

                // Update signal
                if (m_diffusion->inRange(ab2))
                {
                    m_diffusion->getSignalBack(id, ab2) += signal * tmp[ab];
                    assert(m_diffusion->getSignalBack(id, ab2) >= Zero);
                }
            }
        }
    }

    {
#ifdef CECE_THREAD_SAFE
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
}

/* ************************************************************************ */
