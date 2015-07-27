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
#include "core/StaticMatrix.hpp"
#include "core/constants.hpp"
#include "core/TimeMeasurement.hpp"
#include "core/VectorRange.hpp"
#include "simulator/Simulation.hpp"

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
    m_colors.push_back(g_colors[id]);
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

#ifdef THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    // Update all signals
    // TODO: use OpenMP
    for (auto id : getSignalIds())
        updateSignal(step, dt, id);
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
            signal.get<DegradationRate>("degradation-rate", DegradationRate{})
        );

#if ENABLE_RENDER
        // Set signal color
        // FIXME: limited number of colors
        setSignalColor(id, signal.get("color", g_colors[id]));
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
    m_drawable->draw(context);
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::updateDrawable() const
{
    assert(getGridSize() == m_drawable->getSize());

#ifdef THREAD_SAFE
    // Lock access
    MutexGuard guard(m_mutex);
#endif

    // Foreach grid
    for (auto&& c : range(getGridSize()))
    {
        // Initial pixel colour
        auto pixel = m_background;

        // Mixup signal colors
        for (auto id : getSignalIds())
        {
            const auto signal = std::min(getSignal(id, c), Signal(1));

            pixel *= (1 - signal);
            pixel += m_colors[id] * signal;
        }

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

    // Create distribution matrix
    const auto M = StaticMatrix<units::Area, MATRIX_SIZE>::generate([&](size_t i, size_t j) {
        return A * std::exp(-q[i][j] / (4.f * getDiffusionRate(id) * dt));
    }).normalized();

    // Forech grid without borders
    for (auto&& c : range(getGridSize()))
    {
        // Get current signal
        auto signal = getSignalFront(id, c);

        // Nothing to diffuse
        if (signal < std::numeric_limits<Signal>::epsilon())
            continue;

        // Degrade signal
        signal *= 1 - getDegradationRate(id) * dt;

        // Diffuse signal
        for (auto&& ab : range(Coordinate{2 * OFFSET + 1}))
        {
            getSignalBack(id, c + ab - OFFSET) += signal * M[ab];
        }
    }

    // Swap grids
    swap(id);
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
