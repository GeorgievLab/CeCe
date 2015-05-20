
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

// Module
#include "Module.hpp"
#include "Generator.hpp"
#include "GeneratorCell.hpp"

/* ************************************************************************ */

static void generate_signal(simulator::Simulation& simulation, simulator::Object& obj, units::Duration dt, int signal)
{
    constexpr float SOURCE_STRENGTH = 0.2f;

    auto diff = simulation.useModule<module::diffusion::Module>("diffusion");
    assert(diff);
    auto& grid = diff->getGrid();

    const Vector<float> start = simulation.getWorldSize() * -0.5;
    const auto step = simulation.getWorldSize() / grid.getSize();

    // Get cell position
    const auto pos = obj.getPosition() - start;

    // Check if position is in range
    if (!pos.inRange(Vector<float>{0}, simulation.getWorldSize()))
        return;

    // Get grid position
    Vector<unsigned int> coord = pos / step;

    // TODO: use vector + unique (faster?)
    std::set<decltype(coord)> coords;
    const auto& shapes = obj.getShapes();
    auto coordIt = std::inserter(coords, coords.end());

    for (const auto& shape : shapes)
    {
        coordIt = simulator::mapShapeToGrid(coordIt, shape, step, coord, grid.getSize());
    }

    for (const auto& c : coords)
    {
        // Add signal
        grid[c][signal] += SOURCE_STRENGTH * dt;
    }
}

/* ************************************************************************ */

static void remove_signal(simulator::Simulation& simulation, simulator::Object& obj, units::Duration dt, int signal)
{
    constexpr float SOURCE_STRENGTH = 0.2f;

    auto diff = simulation.useModule<module::diffusion::Module>("diffusion");
    assert(diff);
    auto& grid = diff->getGrid();

    const Vector<float> start = simulation.getWorldSize() * -0.5;
    const auto step = simulation.getWorldSize() / grid.getSize();

    // Get cell position
    const auto pos = obj.getPosition() - start;

    // Check if position is in range
    if (!pos.inRange(Vector<float>{0}, simulation.getWorldSize()))
        return;

    // Get grid position
    Vector<unsigned int> coord = pos / step;

    // TODO: use vector + unique (faster?)
    std::set<decltype(coord)> coords;
    const auto& shapes = obj.getShapes();
    auto coordIt = std::inserter(coords, coords.end());

    for (const auto& shape : shapes)
    {
        coordIt = simulator::mapShapeToGrid(coordIt, shape, step, coord, grid.getSize());
    }

    for (const auto& c : coords)
    {
        // Add signal
        grid[c][signal] = std::max(grid[c][signal] - SOURCE_STRENGTH * dt, 0.f);
    }
}

/* ************************************************************************ */

class DiffusionApi : public simulator::LibraryApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) noexcept override
    {
        if (name == "generator")
            return std::unique_ptr<simulator::Module>(new module::diffusion::Generator{simulation.useModule<module::diffusion::Module>("diffusion")});

        if (name == "generator-cell")
            return std::unique_ptr<simulator::Module>(new module::diffusion::GeneratorCell{simulation.useModule<module::diffusion::Module>("diffusion")});

        return std::unique_ptr<simulator::Module>(new module::diffusion::Module{});
    }

    simulator::Program createProgram(simulator::Simulation& simulation, const std::string& name) noexcept override
    {
        if (name == "diffusion.gen1")
            return [&simulation](simulator::Object& obj, units::Duration dt) {
                generate_signal(simulation, obj, dt, 0);
            };

        if (name == "diffusion.gen2")
            return [&simulation](simulator::Object& obj, units::Duration dt) {
                generate_signal(simulation, obj, dt, 1);
            };

        if (name == "diffusion.gen3")
            return [&simulation](simulator::Object& obj, units::Duration dt) {
                generate_signal(simulation, obj, dt, 2);
            };

        if (name == "diffusion.gen4")
            return [&simulation](simulator::Object& obj, units::Duration dt) {
                generate_signal(simulation, obj, dt, 3);
            };

        if (name == "diffusion.rem1")
            return [&simulation](simulator::Object& obj, units::Duration dt) {
                remove_signal(simulation, obj, dt, 0);
            };

        if (name == "diffusion.rem2")
            return [&simulation](simulator::Object& obj, units::Duration dt) {
                remove_signal(simulation, obj, dt, 1);
            };

        if (name == "diffusion.rem3")
            return [&simulation](simulator::Object& obj, units::Duration dt) {
                remove_signal(simulation, obj, dt, 2);
            };

        if (name == "diffusion.rem4")
            return [&simulation](simulator::Object& obj, units::Duration dt) {
                remove_signal(simulation, obj, dt, 3);
            };
    }

};

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_IMPL(DiffusionApi)

/* ************************************************************************ */
