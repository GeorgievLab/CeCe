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
#include "cece/plugins/diffusion-streamlines/Module.hpp"

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Assert.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/StaticMatrix.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/TimeMeasurement.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugins
#include "cece/plugins/streamlines/Module.hpp"
#include "cece/plugins/streamlines/Lattice.hpp"
#include "cece/plugins/streamlines/Dynamics.hpp"
#include "cece/plugins/streamlines/NoDynamics.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion_streamlines {

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    auto _ = measure_time("diffusion-streamlines.loadConfig", simulator::TimeMeasurement(getSimulation()));

    diffusion::Module::loadConfig(config);

    setInnerIterations(config.get("inner-iterations", getInnerIterations()));
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    auto _ = measure_time("diffusion-streamlines.storeConfig", simulator::TimeMeasurement(getSimulation()));

    diffusion::Module::storeConfig(config);

    config.set("inner-iterations", getInnerIterations());
}

/* ************************************************************************ */

void Module::init()
{
    auto _ = measure_time("diffusion-streamlines.init", simulator::TimeMeasurement(getSimulation()));

    diffusion::Module::init();

    m_streamlines = getSimulation().getModule("streamlines");
    Assert(m_streamlines);

    // Get lattice
    const auto& lattice = m_streamlines->getLattice();

    if (lattice.getSize() != getGridSize())
    {
        Log::warning("[diffusion-streamlines] Different grid sizes");
        setGridSize(lattice.getSize());
    }
}

/* ************************************************************************ */

void Module::updateSignal(SignalId id)
{
    auto _ = measure_time("diffusion-streamlines.updateSignal", simulator::TimeMeasurement(getSimulation()));

    // Update diffusion
    diffusion::Module::updateSignal(id);

    Assert(m_streamlines);
    const auto& lattice = m_streamlines->getLattice();
    const auto& conv = m_streamlines->getConverter();

    // Precompute values
    const auto step = getSimulation().getWorldSize() / getGridSize();
    const auto dt = getSimulation().getTimeStep() / getInnerIterations();

    // Same grid sizes
    Assert(lattice.getSize() == getGridSize());

    for (IterationType i = 0; i < getInnerIterations(); ++i)
    {
        clearBack(id);

        // Foreach grid
        for (auto&& c : range(getGridSize()))
        {
            // In obstacle there is no (or shouldn't be) signal
            if (isObstacle(c))
                continue;

            // Get current signal
            const auto signal = getSignal(id, c);

            // No signal to "stream"
            if (!signal)
                continue;

            // Get velocity
            const auto& velocity = conv.convertVelocity(lattice[c].computeVelocity());

            // Distance change
            const auto ds = velocity * dt;

            Assert(ds.getX() <= step.getX());
            Assert(ds.getY() <= step.getY());

            // Transformation matrix
            const auto matrix = StaticMatrix<units::Area, MATRIX_SIZE>::generate([&](size_t i, size_t j) {
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
                Assert(inRange(coord));
                getSignalBack(id, coord) += signal * value;
                Assert(getSignalBack(id, coord) >= Zero);
            });
        }

        // Swap buffers
        swap(id);
    }
}

/* ************************************************************************ */

void Module::updateObstacles()
{
    auto _ = measure_time("diffusion-streamlines.updateObstacles", simulator::TimeMeasurement(getSimulation()));

    // Copy data from streamlines
    Assert(m_streamlines);

    const auto& lattice = m_streamlines->getLattice();
    const auto& noDynamics = streamlines::NoDynamics::getInstance();
    const auto& wallDynamics = m_streamlines->getWallDynamics();

    // Foreach nodes
    for (auto&& c : range(lattice.getSize()))
    {
        const auto& node = lattice[c];
        const auto& dynamics = node.getDynamics();

        const bool flag = dynamics == noDynamics || dynamics == wallDynamics;

        // Set flag
        setObstacle(c, flag);
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
