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
#include <limits>
#include <algorithm>
#include <functional>

// Simulator
#include "core/constants.hpp"
#include "core/StaticMatrix.hpp"
#include "core/TimeMeasurement.hpp"
#include "core/VectorRange.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/ShapeToGrid.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#if ENABLE_RENDER
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

#if ENABLE_RENDER
    m_colors.push_back(g_colors[id % g_colors.size()]);
    m_signalSaturation.push_back(SignalConcentration{1});
#endif

    return id;
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    if (getGridSize() == Zero)
        throw RuntimeException("Diffusion grid size is not set!");

    auto _ = measure_time("diffusion", simulator::TimeMeasurementIterationOutput(simulation));

    // Precompute values
    const auto step = simulation.getWorldSize() / getGridSize();

    // Update obstacle map from scene
    updateObstacles(simulation);

    // For thread safety it is divided to two parts
    // The first part update diffusion to back buffer that is not used elsewhere,
    // so it's perfectly safe (it's not a critical section)
    // The second part just swap buffers - it's critical section but it's fast.

    // Update all signals
    // TODO: use OpenMP
    for (auto id : getSignalIds())
        updateSignal(step, dt, id);

    {
#if THREAD_SAFE
        // Lock access
        MutexGuard guard(m_mutex);
#endif
        // Swap grids
        swapAll();
    }
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Grid size
    setGridSize(config.get<Vector<GridType::SizeType>>("grid"));

    // Foreach signal configurations
    for (auto&& signal : config.getConfigurations("signal"))
    {
        // Register signal
        auto id = registerSignal(
            signal.get("name"),
            signal.get<DiffusionRate>("diffusion-rate"),
            signal.get<DegradationRate>("degradation-rate", Zero)
        );

#if ENABLE_RENDER
        // Set signal color
        setSignalColor(id, signal.get("color", getSignalColor(id)));
        setSignalSaturation(id, signal.get("saturation", getSignalSaturation(id)));
#endif
    }

#if ENABLE_RENDER
    // Set background color
    m_background = config.get("background", m_background);
#endif
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    if (getGridSize() == Zero)
        throw RuntimeException("Diffusion grid size is not set!");

    if (!m_drawable)
        m_drawable.create(context, getGridSize());

    // Update drawable
    updateDrawable();

    context.matrixPush();
    context.matrixScale(simulation.getWorldSize() / units::Length(1));
    context.colorPush();
    context.enableAlpha();
    m_drawable->draw(context);
    context.disableAlpha();
    context.colorPop();
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::updateDrawable() const
{
    assert(m_drawable);
    assert(getGridSize() == m_drawable->getSize());

#if THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    // Foreach grid
    for (auto&& c : range(getGridSize()))
    {
        RealType alphaSum = 0;

        for (auto id : getSignalIds())
            alphaSum += std::min<RealType>(getSignal(id, c) / getSignalSaturation(id), 1);

        // Alpha for background
        const auto alphaBg = std::max<RealType>(1 - alphaSum, 0);

        // Initial pixel colour
        auto pixel = m_background * alphaBg;

        if (alphaSum)
        {
            render::Color pixelSignals{};

            // Mixup signal colors
            for (auto id : getSignalIds())
            {
                // Calculate alpha value
                const auto alpha = std::min<RealType>(getSignal(id, c) / getSignalSaturation(id), 1);

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

void Module::updateSignal(const PositionVector& step, units::Time dt, SignalId id)
{
    // Size of mapping matrix
    constexpr unsigned MATRIX_SIZE = 2 * OFFSET + 1;

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
        for (auto&& ab : range(Coordinate{MATRIX_SIZE}))
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

            for (auto&& ab : range(Coordinate{MATRIX_SIZE}))
            {
                const auto coord = c + ab - OFFSET;

                if (!isObstacle(coord))
                    getSignalBack(id, coord) += signalAdd;
            }
        }
    }
}

/* ************************************************************************ */

void Module::updateObstacles(simulator::Simulation& simulation)
{
    // Clear previous flag
    std::fill(m_obstacles.begin(), m_obstacles.end(), false);

    const PositionVector start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / getGridSize();

    // Foreach all cells
    for (auto& obj : simulation.getObjects())
    {
        // Ignore non-static objects
        if (obj->getType() == simulator::Object::Type::Dynamic)
            continue;

        // Get object position
        const auto pos = obj->getPosition() - start;

        // Check if position is in range
        if (!pos.inRange(Zero, simulation.getWorldSize()))
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

/* ************************************************************************ */
