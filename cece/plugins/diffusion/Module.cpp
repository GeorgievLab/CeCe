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
#include "cece/plugins/diffusion/Module.hpp"

// C++
#include <limits>
#include <algorithm>
#include <functional>

// CeCe
#include "cece/core/constants.hpp"
#include "cece/core/Assert.hpp"
#include "cece/core/StaticMatrix.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/core/FileStream.hpp"
#include "cece/core/ShapeToGrid.hpp"
#include "cece/simulator/TimeMeasurement.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
constexpr StaticArray<render::Color, 6> g_colors{{
    render::colors::CYAN,
    render::colors::MAGENTA,
    render::colors::YELLOW,
    render::colors::BLUE,
    render::colors::RED,
    render::colors::GREEN
}};
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

void Module::setGridSize(SizeType size)
{
    m_gridSize = std::move(size);

    // Resize current grids
    for (auto id : getSignalIds())
    {
        m_gridsFront[id].resize(m_gridSize + 2 * OFFSET);
        m_gridsBack[id].resize(m_gridSize + 2 * OFFSET);
    }

    m_obstacles.resize(m_gridSize + 2 * OFFSET, false);
}

/* ************************************************************************ */

Module::SignalId Module::registerSignal(String name, DiffusionRate rate, DegradationRate degRate)
{
    const SignalId id = m_names.size();

    m_names.push_back(std::move(name));
    m_diffusionRates.push_back(rate);
    m_degradationRates.push_back(degRate);
    m_gridsFront.emplace_back(m_gridSize + 2 * OFFSET);
    m_gridsBack.emplace_back(m_gridSize + 2 * OFFSET);

#ifdef CECE_ENABLE_RENDER
    m_colors.push_back(g_colors[id % g_colors.size()]);
    m_signalSaturation.push_back(SignalConcentration{1});
#endif

    return id;
}

/* ************************************************************************ */

void Module::update()
{
    if (getGridSize() == Zero)
        throw RuntimeException("Diffusion grid size is not set!");

    auto _ = measure_time("diffusion", simulator::TimeMeasurement(getSimulation()));

    // Precompute values
    const auto step = getSimulation().getWorldSize() / getGridSize();

    // Update obstacle map from scene
    updateObstacles();

    // For thread safety it is divided to two parts
    // The first part update diffusion to back buffer that is not used elsewhere,
    // so it's perfectly safe (it's not a critical section)
    // The second part just swap buffers - it's critical section but it's fast.

    // Update all signals
    // TODO: use OpenMP
    for (auto id : getSignalIds())
        updateSignal(step, id);

    {
#ifdef CECE_THREAD_SAFE
        // Lock access
        MutexGuard guard(m_mutex);
#endif
        // Swap grids
        swapAll();
    }

    // Store streamlines data
    if (m_dataOut)
    {
        for (auto&& c : range(getGridSize()))
        {
            *m_dataOut <<
                // iteration
                getSimulation().getIteration() << ";" <<
                // totalTime
                getSimulation().getTotalTime().value() << ";" <<
                // x
                c.getX() << ";" <<
                // y
                c.getY();
            ;

            // Store signal concentrations
            for (auto id : getSignalIds())
                *m_dataOut << ";" << getSignal(id, c);

            *m_dataOut << "\n";
        }

        m_dataOut->flush();
    }
}

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    // Configure parent
    module::Module::loadConfig(config);

    // Grid size
    setGridSize(config.get<Vector<GridType::SizeType>>("grid"));

    // Foreach signal configurations
    for (auto&& signal : config.getConfigurations("signal"))
    {
        // Register signal
#ifdef CECE_ENABLE_RENDER
        auto id =
#endif
        registerSignal(
            signal.get("name"),
            signal.get<DiffusionRate>("diffusion-rate"),
            signal.get<DegradationRate>("degradation-rate", Zero)
        );

#ifdef CECE_ENABLE_RENDER
        // Set signal color
        setSignalColor(id, signal.get("color", getSignalColor(id)));
        setSignalSaturation(id, signal.get("saturation", getSignalSaturation(id)));
#endif
    }

#ifdef CECE_ENABLE_RENDER
    // Set background color
    m_background = config.get("background", m_background);
#endif

    Log::info("[diffusion] Grid size: ", getGridSize().getWidth(), "x", getGridSize().getHeight());

    for (auto id : getSignalIds())
    {
        Log::info("[diffusion] Signal '", getSignalName(id), "':");
        Log::info("[diffusion]   Diffusion rate: ", getDiffusionRate(id), " um2/s");
        Log::info("[diffusion]   Degradation rate: ", getDegradationRate(id), " /s");

#ifdef CECE_ENABLE_RENDER
        Log::info("[diffusion]   Color: ", getSignalColor(id));
        Log::info("[diffusion]   Saturation: ", getSignalSaturation(id), " umol/um3");
#endif
    }

    if (config.has("data-out"))
    {
        m_dataOut = makeUnique<OutFileStream>(config.get("data-out"));
        *m_dataOut << "iteration;totalTime;x;y";

        // Store signal names
        for (auto id : getSignalIds())
            *m_dataOut << ";" << getSignalName(id);

        *m_dataOut << "\n";
    }
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    module::Module::storeConfig(config);

    config.set("grid", getGridSize());

    // Foreach signal configurations
    for (std::size_t i = 0; i < getSignalCount(); ++i)
    {
        auto signalConfig = config.addConfiguration("signal");
        signalConfig.set("name", m_names[i]);
        signalConfig.set("diffusion-rate", m_diffusionRates[i]);
        signalConfig.set("degradation-rate", m_degradationRates[i]);

#ifdef CECE_ENABLE_RENDER
        signalConfig.set("color", m_colors[i]);
        signalConfig.set("saturation", m_signalSaturation[i]);
#endif
    }

#ifdef CECE_ENABLE_RENDER
    // Set background color
    config.set("background", m_background);
#endif
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(render::Context& context)
{
    if (getGridSize() == Zero)
        throw RuntimeException("Diffusion grid size is not set!");

    if (!m_drawable)
        m_drawable.create(context, getGridSize());

    // Update drawable
    updateDrawable();

    context.matrixPush();
    context.matrixScale(getSimulation().getWorldSize() / units::Length(1));
    context.colorPush();
    context.enableAlpha();
    m_drawable->draw(context);
    context.disableAlpha();
    context.colorPop();
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::updateDrawable() const
{
    assert(m_drawable);
    assert(getGridSize() == m_drawable->getSize());

#ifdef CECE_THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    // Foreach grid
    for (auto&& c : range(getGridSize()))
    {
        RealType alphaSum = 0;

        for (auto id : getSignalIds())
        {
            if (m_colors[id] != render::colors::TRANSPARENT)
                alphaSum += std::min<RealType>(getSignal(id, c) / getSignalSaturation(id), 1);
        }

        Assert(alphaSum >= 0);

        // Alpha for background
        const auto alphaBg = std::max<RealType>(1 - alphaSum, 0);
        Assert(alphaBg >= 0);

        // Initial pixel colour
        auto pixel = m_background * alphaBg;

        if (alphaSum > 0)
        {
            render::Color pixelSignals;

            // Mixup signal colors
            for (auto id : getSignalIds())
            {
                if (m_colors[id] == render::colors::TRANSPARENT)
                    continue;

                // Calculate alpha value
                const auto alpha = std::min<RealType>(getSignal(id, c) / getSignalSaturation(id), 1);
                Assert(alpha >= 0);

                pixelSignals += m_colors[id] * alpha / alphaSum;
            }

            pixel += (1 - alphaBg) * pixelSignals;
        }

#if DEV_PLUGIN_diffusion_OBSTACLES_RENDER
        if (isObstacle(c))
            pixel = render::colors::BLUE;
#endif

        m_drawable->set(c, pixel);
    }
}
#endif

/* ************************************************************************ */

void Module::updateSignal(const PositionVector& step, SignalId id)
{
    // Size of mapping matrix
    constexpr unsigned MATRIX_SIZE = 2 * OFFSET + 1;

    // Time step
    const auto dt = getSimulation().getTimeStep();

    // Clear back grid
    clearBack(id);

    // Amplitude
    const auto A = 1.f / (4 * constants::PI * getDiffusionRate(id) * dt);

    // Offset coefficients for matrix
    static const auto DISTANCES = StaticMatrix<int, MATRIX_SIZE>::makeDistances();

    // Generate matrix with coefficients based on distance
    const auto q = StaticMatrix<units::Area, MATRIX_SIZE>::generate([&step](size_t i, size_t j) {
        return (step * DISTANCES[i][j]).getLengthSquared();
    });

    using MT = units::Inverse<units::Area>::type;

    // Create distribution matrix
    const auto M = StaticMatrix<MT, MATRIX_SIZE>::generate([&](size_t i, size_t j) {
        return A * std::exp(-q[i][j] / (4.f * getDiffusionRate(id) * dt));
    }).normalized();

    // Forech grid without borders
    for (auto&& c : range(getGridSize()))
    {
        // Get current signal
        auto signal = getSignalFront(id, c);

        // Nothing to diffuse
        if (signal < std::numeric_limits<SignalConcentration>::epsilon())
            continue;

        // Degrade signal
        signal *= 1 - getDegradationRate(id) * dt;

        SignalConcentration obstacleSignal = Zero;
        unsigned int obstacleCells = 0u;

        // Diffuse signal to grid cells around
        for (auto&& ab : range(Coordinate::createSingle(MATRIX_SIZE)))
        {
            const auto coord = c + ab - OFFSET;

            if (isObstacle(coord))
            {
                ++obstacleCells;
                obstacleSignal += signal * M[ab];
            }
            else
            {
                getSignalBack(id, coord) += signal * M[ab];
            }
        }

        // Only in case there is obstacles
        if (obstacleCells > 0)
        {
            assert(signal >= Zero);

            // Divide obstacle signal between non-obstacle grid cells
            const SignalConcentration signalAdd = obstacleSignal / (MATRIX_SIZE * MATRIX_SIZE - obstacleCells);
            assert(signalAdd >= Zero);

            for (auto&& ab : range(Coordinate::createSingle(MATRIX_SIZE)))
            {
                const auto coord = c + ab - OFFSET;

                if (!isObstacle(coord))
                    getSignalBack(id, coord) += signalAdd;
            }
        }
    }
}

/* ************************************************************************ */

void Module::updateObstacles()
{
    // Clear previous flag
    std::fill(m_obstacles.begin(), m_obstacles.end(), false);

    const PositionVector start = getSimulation().getWorldSize() * -0.5f;
    const auto step = getSimulation().getWorldSize() / getGridSize();

    // Foreach all cells
    for (auto& obj : getSimulation().getObjects())
    {
        // Ignore non-static objects
        if (obj->getType() == object::Object::Type::Dynamic)
            continue;

        // Get object position
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(Zero, getSimulation().getWorldSize()))
            continue;

        // Get grid position
        const auto coord = Coordinate(pos / step);

        // In this case duplicate coordinates doesn't harm and calling
        // operation multiple times on same coordinate is faster than
        // sorting and erasing non-unique coordinates.

        // Map shapes to grid
        for (const auto& shape : obj->getShapes())
        {
            mapShapeToGrid(
                [this] (Coordinate&& coord) {
                    assert(m_obstacles.inRange(coord + OFFSET));
                    m_obstacles[coord + OFFSET] = true;
                },
                [] (Coordinate&& coord) {},
                shape, step, coord, obj->getRotation(), getGridSize()
            );
        }
    }
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
