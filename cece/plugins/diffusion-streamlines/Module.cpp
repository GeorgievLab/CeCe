/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include "cece/core/Log.hpp"
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
    module::Module::loadConfig(config);

    setInnerIterations(config.get("inner-iterations", getInnerIterations()));
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    module::Module::storeConfig(config);

    config.set("inner-iterations", getInnerIterations());
}

/* ************************************************************************ */

void Module::init()
{
    m_streamlines = getSimulation().getModule("streamlines");
    m_diffusion = getSimulation().getModule("diffusion");

    Assert(m_streamlines);
    Assert(m_diffusion);

    if (m_streamlines->getLattice().getSize() != m_diffusion->getGridSize())
    {
        Log::warning("[diffusion-streamlines] Different grid sizes");
        m_diffusion->setGridSize(m_streamlines->getLattice().getSize());
    }
}

/* ************************************************************************ */

void Module::update()
{
    auto _ = measure_time("diffusion-streamlines", simulator::TimeMeasurement(getSimulation()));

    using Coordinate = diffusion::Module::Coordinate;

    /// Matrix offset
    constexpr int OFFSET = 1;
    constexpr int SIZE = OFFSET * 2 + 1;

    Assert(m_streamlines);
    Assert(m_diffusion);

    const auto signalGridSize = m_diffusion->getGridSize();
    const auto& lattice = m_streamlines->getLattice();
    const auto& conv = m_streamlines->getConverter();

    // Precompute values
    const auto step = getSimulation().getWorldSize() / signalGridSize;

    // Time step
    const auto dt = getSimulation().getTimeStep() / getInnerIterations();

    // Same grid sizes
    Assert(lattice.getSize() == m_diffusion->getGridSize());

    for (simulator::IterationNumber i = 0; i < getInnerIterations(); ++i)
    {
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

                // Get velocity
                const auto& velocity = conv.convertVelocity(lattice[c].computeVelocity());

                // Distance change
                const auto ds = velocity * dt;

                Assert(ds.getX() <= step.getX());
                Assert(ds.getY() <= step.getY());

                // Transformation matrix
                const auto matrix = StaticMatrix<units::Area, SIZE>::generate([&](size_t i, size_t j) {
                    using std::abs;
                    using std::max;

                    const int i2 = (static_cast<int>(i) - OFFSET);
                    const int j2 = (static_cast<int>(j) - OFFSET);

                    const auto x = (j2 == 0)
                        ? (step.getX() - abs(ds.getX()))
                        : max(j2 * ds.getX(), units::Length(0))
                    ;

                    const auto y = (i2 == 0)
                        ? (step.getX() - abs(ds.getY()))
                        : max(i2 * ds.getY(), units::Length(0))
                    ;

                    return x * y;
                }).normalized();

                // Apply matrix
                matrix.for_each([&](size_t i, size_t j, RealType value) {
                    const auto coord = c + Coordinate(j, i) - OFFSET;
                    Assert(m_diffusion->inRange(coord));
                    m_diffusion->getSignalBack(id, coord) += signal * value;
                    Assert(m_diffusion->getSignalBack(id, coord) >= Zero);
                });
            }
        }

        {
#ifdef CECE_THREAD_SAFE
            // Lock diffusion
            MutexGuard guard(m_diffusion->getMutex());
#endif

            // Swap buffers
            for (auto id : m_diffusion->getSignalIds())
                m_diffusion->swap(id);
        }
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
