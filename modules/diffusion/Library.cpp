
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

DEFINE_LIBRARY_INIT(simulation)
{
    assert(simulation);

    simulation->addProgram("diffusion.gen1", [simulation](simulator::Object& obj, units::Duration dt) {
        generate_signal(*simulation, obj, dt, 0);
    });

    simulation->addProgram("diffusion.gen2", [simulation](simulator::Object& obj, units::Duration dt) {
        generate_signal(*simulation, obj, dt, 1);
    });

    simulation->addProgram("diffusion.gen3", [simulation](simulator::Object& obj, units::Duration dt) {
        generate_signal(*simulation, obj, dt, 2);
    });

    simulation->addProgram("diffusion.gen4", [simulation](simulator::Object& obj, units::Duration dt) {
        generate_signal(*simulation, obj, dt, 3);
    });

    simulation->addProgram("diffusion.rem1", [simulation](simulator::Object& obj, units::Duration dt) {
        remove_signal(*simulation, obj, dt, 0);
    });

    simulation->addProgram("diffusion.rem2", [simulation](simulator::Object& obj, units::Duration dt) {
        remove_signal(*simulation, obj, dt, 1);
    });

    simulation->addProgram("diffusion.rem3", [simulation](simulator::Object& obj, units::Duration dt) {
        remove_signal(*simulation, obj, dt, 2);
    });

    simulation->addProgram("diffusion.rem4", [simulation](simulator::Object& obj, units::Duration dt) {
        remove_signal(*simulation, obj, dt, 3);
    });
}

/* ************************************************************************ */

DEFINE_LIBRARY_FINALIZE(simulation)
{
    // Nothing to do
}

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_MODULE(simulation, name)
{
    if (!strcmp(name, "generator"))
        return new module::diffusion::Generator{simulation->useModule<module::diffusion::Module>("diffusion")};

    if (!strcmp(name, "generator-cell"))
        return new module::diffusion::GeneratorCell{simulation->useModule<module::diffusion::Module>("diffusion")};

    return new module::diffusion::Module{};
}

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE_OBJECT(simulation, name, flags)
{
    return nullptr;
}

/* ************************************************************************ */
