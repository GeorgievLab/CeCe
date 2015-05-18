
// Declaration
#include "GeneratorCell.hpp"

// C++
#include <cassert>
#include <set>

// Simulator
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Shape.hpp"
#include "simulator/ShapeToGrid.hpp"

// Module
#include "Module.hpp"

// Module "cell"
#include "../cell/Yeast.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

void GeneratorCell::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("diffusion.generator-cell", [&simulation](std::ostream& out, const std::string& name, Clock::duration dt) {
        out << name << ";" << simulation.getStepNumber() << ";" << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << "\n";
    });

    constexpr float SOURCE_STRENGTH = 1.f;

    assert(m_diffusionModule);
    auto& grid = m_diffusionModule->getGrid();

    const Vector<float> start = simulation.getWorldSize() * -0.5;
    const auto step = simulation.getWorldSize() / grid.getSize();

    // Foreach all cells
    for (auto& obj : simulation.getObjects())
    {
        if (obj->getType() == simulator::Object::Type::Static)
            continue;

        // It's not cell
        if (!obj->is<module::cell::Yeast>())
            continue;

        // Cast to cell
        auto ptr = obj->cast<module::cell::Yeast>();

        // Get cell position
        const auto pos = ptr->getPosition() - start;
        const unsigned int signal = ptr->getId() % Signal::COUNT;

        // Check if position is in range
        if (!pos.inRange(Vector<float>{0}, simulation.getWorldSize()))
            continue;

        // Get grid position
        Vector<unsigned int> coord = pos / step;

        // TODO: use vector + unique (faster?)
        std::set<decltype(coord)> coords;
        auto shapes = static_cast<const simulator::Object*>(obj.get())->getShapes();
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
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
