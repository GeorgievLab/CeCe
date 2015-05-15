
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

template<typename T>
static void shapeCoordinates(const Vector<T>& center, const simulator::Shape& shape,
    const Vector<float>& step, std::set<Vector<T>>& coords)
{
    if (shape.type != simulator::ShapeType::Circle)
        return;

    const Vector<T> radius = shape.circle.radius / step;

    for (T x = -radius.getX(); x < radius.getX(); ++x)
    {
        for (T y = -radius.getY(); y < radius.getY(); ++y)
        {
            const Vector<T> xy{x, y};
            auto len = xy.getLength();

            if (len > radius.getX() || (len < radius.getX() - 2))
                continue;

            // New coordinate
            auto coord = center + shape.circle.center + xy;

            if (coord.getX() < 0 || coord.getY() < 0)
                continue;

            coords.insert(coord);
        }
    }
}

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

        // TODO: improve
        if ((pos.getX() < 0 || pos.getY() < 0) ||
            (pos.getX() >= simulation.getWorldSize().getWidth() || pos.getY() >= simulation.getWorldSize().getHeight()))
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
