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
#include "cece/plugins/diffusion/Module.hpp"

// C++
#include <algorithm>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/StaticMatrix.hpp"
#include "cece/core/TimeMeasurement.hpp"
#include "cece/core/VectorRange.hpp"
#include "cece/core/ShapeToGrid.hpp"
#include "cece/core/UnitIo.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/object/Object.hpp"
#include "cece/simulator/TimeMeasurement.hpp"
#include "cece/simulator/Simulation.hpp"
#include "cece/simulator/Visualization.hpp"

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

Module::SignalId Module::getSignalId(StringView name) const noexcept
{
    for (auto id : getSignalIds())
    {
        if (m_signals[id].name == name)
            return id;
    }

    return INVALID_SIGNAL_ID;
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
    const SignalId id = m_signals.size();

    m_signals.push_back(Signal{
        std::move(name),
        rate,
        degRate
#ifdef CECE_ENABLE_RENDER
        , g_colors[id % g_colors.size()]
        , SignalConcentration{1}
        , String{}
#endif
    });

    m_gridsFront.emplace_back(m_gridSize + 2 * OFFSET);
    m_gridsBack.emplace_back(m_gridSize + 2 * OFFSET);

    return id;
}

/* ************************************************************************ */

void Module::loadConfig(const config::Configuration& config)
{
    auto _ = measure_time("diffusion.loadConfig", simulator::TimeMeasurement(getSimulation()));

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
        setSignalVisualizationLayer(id, signal.get("layer", String{}));
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
}

/* ************************************************************************ */

void Module::storeConfig(config::Configuration& config) const
{
    auto _ = measure_time("diffusion.storeConfig", simulator::TimeMeasurement(getSimulation()));

    module::Module::storeConfig(config);

    config.set("grid", getGridSize());

    // Foreach signal configurations
    for (auto id : getSignalIds())
    {
        auto signalConfig = config.addConfiguration("signal");
        signalConfig.set("name", getSignalName(id));
        signalConfig.set("diffusion-rate", getDiffusionRate(id));
        signalConfig.set("degradation-rate", getDegradationRate(id));

#ifdef CECE_ENABLE_RENDER
        signalConfig.set("color", getSignalColor(id));
        signalConfig.set("saturation", getSignalSaturation(id));
#endif
    }

#ifdef CECE_ENABLE_RENDER
    // Set background color
    config.set("background", m_background);
#endif
}

/* ************************************************************************ */

void Module::init()
{
    auto _ = measure_time("diffusion.init", simulator::TimeMeasurement(getSimulation()));

    if (getGridSize() == Zero)
        throw RuntimeException("Diffusion grid size is not set!");
}

/* ************************************************************************ */

void Module::update()
{
    Assert(getGridSize() != Zero);

    auto _ = measure_time("diffusion.update", simulator::TimeMeasurement(getSimulation()));

    // Update obstacle map from scene
    updateObstacles();

    // Update all signals (MSVC requires signed int)
    #pragma omp parallel for
    for (int id = 0; id < static_cast<int>(getSignalCount()); ++id)
        updateSignal(static_cast<SignalId>(id));
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::draw(const simulator::Visualization& visualization, render::Context& context)
{
    Assert(getGridSize() != Zero);

    auto _ = measure_time("diffusion.draw", simulator::TimeMeasurement(getSimulation()));

    if (!m_drawable)
        m_drawable.create(context, getGridSize());

    // Update drawable
    updateDrawable(visualization);

    context.matrixPush();
    context.matrixScale(getSimulation().getWorldSize() / units::Length(1));
    context.colorPush();
    context.setColor(render::colors::WHITE);
    context.enableAlpha();
    m_drawable->draw(context);
    context.disableAlpha();
    context.colorPop();
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Module::updateDrawable(const simulator::Visualization& visualization) const
{
    Assert(m_drawable);
    Assert(getGridSize() == m_drawable->getSize());

#ifdef CECE_THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    // Drawed signals
    DynamicArray<SignalId> signals;

    for (auto id : getSignalIds())
    {
        const auto layer = getSignalVisualizationLayer(id);

        // Store only when empty or enabled
        if (layer.empty() || visualization.isEnabled(layer))
            signals.push_back(id);
    }

    // Foreach grid
    for (auto&& c : range(getGridSize()))
    {
        RealType alphaSum = 0;

        for (auto id : signals)
        {
            if (getSignalColor(id) != render::colors::TRANSPARENT)
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
            for (auto id : signals)
            {
                if (getSignalColor(id) == render::colors::TRANSPARENT)
                    continue;

                // Calculate alpha value
                const auto alpha = std::min<RealType>(getSignal(id, c) / getSignalSaturation(id), 1);
                Assert(alpha >= 0);

                pixelSignals += getSignalColor(id) * alpha / alphaSum;
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

void Module::swap(SignalId id) noexcept
{
    using std::swap;
    swap(m_gridsFront[id], m_gridsBack[id]);
}

/* ************************************************************************ */

void Module::clearFront(SignalId id) noexcept
{
    std::fill(m_gridsFront[id].begin(), m_gridsFront[id].end(), SignalConcentration{});
}

/* ************************************************************************ */

void Module::clearBack(SignalId id) noexcept
{
    std::fill(m_gridsBack[id].begin(), m_gridsBack[id].end(), SignalConcentration{});
}

/* ************************************************************************ */

void Module::updateSignal(SignalId id)
{
    auto _ = measure_time("diffusion.updateSignal", simulator::TimeMeasurement(getSimulation()));

    // Distance coefficients
    static const auto DISTANCES = StaticMatrix<int, MATRIX_SIZE>::makeDistances();

    // Some used values
    const auto dt = getSimulation().getTimeStep();
    const auto step = getSimulation().getWorldSize() / getGridSize();

    // Generate matrix with coefficients based on distance
    const auto distances = StaticMatrix<units::Area, MATRIX_SIZE>::generate([&step](size_t i, size_t j) {
        return (step * DISTANCES[i][j]).getLengthSquared();
    });

    // Precompute convolution matrix
    const auto convMatrix = StaticMatrix<RealType, MATRIX_SIZE>::generate([&](size_t i, size_t j) {
        return std::exp(-distances[i][j] / (RealType(4.0) * getDiffusionRate(id) * dt));
    });

    // Clear back grid
    clearBack(id);

    // Forech grid without borders
    for (auto&& c : range(getGridSize()))
    {
        // In obstacle there is no (or shouldn't be) signal
        if (isObstacle(c))
            continue;

        // Get current signal
        auto signal = getSignalFront(id, c);

        // Nothing to diffuse
        if (!signal)
            continue;

        // Degrade signal
        signal *= RealType(1) - getDegradationRate(id) * dt;

        // Modify matrix by obstacle presentation
        const auto matrix = convMatrix.transform([&](size_t i, size_t j, RealType value) {
            const auto coord = c + Coordinate(j, i) - OFFSET;
            return isObstacle(coord) ? RealType(0) : value;
        }).normalized();

        // Diffuse signal to grid cells around
        matrix.for_each([&](size_t i, size_t j, RealType value) {
            const auto coord = c + Coordinate(j, i) - OFFSET;
            getSignalBack(id, coord) += signal * value;
            Assert(getSignalBack(id, coord) >= Zero);
        });
    }

    // Swap buffers
    swap(id);
}

/* ************************************************************************ */

void Module::updateObstacles()
{
    auto _ = measure_time("diffusion.updateObstacles", simulator::TimeMeasurement(getSimulation()));

    // Clear previous flag
    std::fill(m_obstacles.begin(), m_obstacles.end(), false);

    const PositionVector start = getSimulation().getWorldSize() * -0.5;
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
                    Assert(m_obstacles.inRange(coord + OFFSET));
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
