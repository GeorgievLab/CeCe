
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// C++
#include <cassert>
#include <cstring>
#include <set>

// Simulator
#include "core/Units.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/ShapeToGrid.hpp"
#include "simulator/Library.hpp"
#include "simulator/PluginApi.hpp"

// Module
#include "Module.hpp"
#include "Generator.hpp"

/* ************************************************************************ */

using namespace simulator;

/* ************************************************************************ */

static void generate_signal(Simulation& simulation, Object& obj, units::Duration dt, int signal)
{
    CONSTEXPR float SOURCE_STRENGTH = 0.2f;

    auto diff = simulation.useModule<plugin::diffusion::Module>("diffusion");
    assert(diff);
    auto& grid = diff->getGrid();

    const auto start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / grid.getSize();

    // Get cell position
    const auto pos = obj.getPosition() - start;

    // Check if position is in range
    if (!pos.inRange(PositionVector::Zero, simulation.getWorldSize()))
        return;

    // Get grid position
    auto coord = Vector<unsigned int>(pos / step);

    // TODO: use vector + unique (faster?)
    std::vector<decltype(coord)> coords;
    const auto& shapes = obj.getShapes();
    auto coordIt = std::inserter(coords, coords.end());

    for (const auto& shape : shapes)
    {
        coordIt = mapShapeToGrid(coordIt, shape, step, coord, grid.getSize());
    }

    std::sort(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
        return lhs < rhs;
    });
    coords.erase(std::unique(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
        return lhs == rhs;
    }), coords.end());

    for (const auto& c : coords)
    {
        // Add signal
        grid[c][signal] += SOURCE_STRENGTH * dt.value();
    }
}

/* ************************************************************************ */

static void remove_signal(Simulation& simulation, Object& obj, units::Duration dt, int signal)
{
    CONSTEXPR float SOURCE_STRENGTH = 0.2f;

    auto diff = simulation.useModule<plugin::diffusion::Module>("diffusion");
    assert(diff);
    auto& grid = diff->getGrid();

    const auto start = simulation.getWorldSize() * -0.5f;
    const auto step = simulation.getWorldSize() / grid.getSize();

    // Get cell position
    const auto pos = obj.getPosition() - start;

    // Check if position is in range
    if (!pos.inRange(PositionVector::Zero, simulation.getWorldSize()))
        return;

    // Get grid position
    auto coord = Vector<unsigned int>(pos / step);

    // TODO: use vector + unique (faster?)
    std::vector<decltype(coord)> coords;
    const auto& shapes = obj.getShapes();
    auto coordIt = std::inserter(coords, coords.end());

    for (const auto& shape : shapes)
    {
        coordIt = mapShapeToGrid(coordIt, shape, step, coord, grid.getSize());
    }

    std::sort(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
        return lhs < rhs;
    });
    coords.erase(std::unique(coords.begin(), coords.end(), [](const decltype(coord)& lhs, const decltype(coord)& rhs) {
        return lhs == rhs;
    }), coords.end());

    for (const auto& c : coords)
    {
        // Add signal
        grid[c][signal] = std::max(grid[c][signal] - SOURCE_STRENGTH * dt.value(), 0.f);
    }
}

/* ************************************************************************ */

class DiffusionApi : public PluginApi
{
    std::unique_ptr<Module> createModule(Simulation& simulation, const String& name) NOEXCEPT override
    {
        if (name == "generator")
            return std::unique_ptr<Module>(new plugin::diffusion::Generator{simulation.useModule<plugin::diffusion::Module>("diffusion")});

        return std::unique_ptr<Module>(new plugin::diffusion::Module{});
    }

    Program createProgram(Simulation& simulation, const String& name, String code = {}) NOEXCEPT override
    {
        if (name == "gen1")
            return [&simulation](Object& obj, units::Duration dt) {
                generate_signal(simulation, obj, dt, 0);
            };

        if (name == "gen2")
            return [&simulation](Object& obj, units::Duration dt) {
                generate_signal(simulation, obj, dt, 1);
            };

        if (name == "gen3")
            return [&simulation](Object& obj, units::Duration dt) {
                generate_signal(simulation, obj, dt, 2);
            };

        if (name == "gen4")
            return [&simulation](Object& obj, units::Duration dt) {
                generate_signal(simulation, obj, dt, 3);
            };

        if (name == "rem1")
            return [&simulation](Object& obj, units::Duration dt) {
                remove_signal(simulation, obj, dt, 0);
            };

        if (name == "rem2")
            return [&simulation](Object& obj, units::Duration dt) {
                remove_signal(simulation, obj, dt, 1);
            };

        if (name == "rem3")
            return [&simulation](Object& obj, units::Duration dt) {
                remove_signal(simulation, obj, dt, 2);
            };

        if (name == "rem4")
            return [&simulation](Object& obj, units::Duration dt) {
                remove_signal(simulation, obj, dt, 3);
            };

        return nullptr;
    }

};

/* ************************************************************************ */

DEFINE_LIBRARY(diffusion, DiffusionApi)

/* ************************************************************************ */
