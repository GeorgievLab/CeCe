
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <vector>

// Simulator
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "core/Grid.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;

/* ************************************************************************ */

/**
 * @brief Shape types.
 */
enum class ShapeType
{
    Circle
};

/* ************************************************************************ */

/**
 * @brief Structure for circle shape.
 */
struct ShapeCircle
{
    /// Shape center.
    PositionVector center;

    /// Circle radius.
    units::Length radius;
};

/* ************************************************************************ */

/**
 * @brief Structure for storing shape.
 */
struct Shape
{
    /// Type of the shape.
    ShapeType type;

    /// Shapes data
    union
    {
        ShapeCircle circle;
    };


    /**
     * @brief Create circle shape.
     *
     * @param radius Circle radius.
     * @param center Circle center (position offset).
     *
     * @return
     */
    static Shape makeCircle(units::Length radius, PositionVector center = {0, 0}) noexcept
    {
        return Shape{ShapeType::Circle, ShapeCircle{center, radius}};
    }

};

/* ************************************************************************ */

/**
 * @brief Map shape to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 */
template<typename OutIt, typename T, typename StepT>
OutIt mapShapeToGrid(OutIt out, const Shape& shape, const Vector<StepT>& steps,
                     const Vector<T>& center,
                     const Vector<T>& max, const Vector<T>& min = {})
{
    switch (shape.type)
    {
    default:
        break;

    case ShapeType::Circle:
        return mapShapeToGrid(out, shape.circle, steps, center, max, min);
    }

    return out;
}

/* ************************************************************************ */

/**
 * @brief Map circle shape to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 */
template<typename OutIt, typename T, typename StepT>
OutIt mapShapeToGrid(OutIt out, const ShapeCircle& shape, const Vector<StepT>& steps,
                     const Vector<T>& center,
                     const Vector<T>& max, const Vector<T>& min = {})
{
    // Radius steps in grid
    const Vector<T> radiusSteps = shape.radius / steps;
    const Vector<T> shapeCenter = center + shape.center / steps;

    // Foreach grid given by radius steps
    for (T x = -radiusSteps.getX(); x < radiusSteps.getX(); ++x)
    {
        for (T y = -radiusSteps.getY(); y < radiusSteps.getY(); ++y)
        {
            // Calculate normalized length
            const Vector<T> xy{x, y};
            const auto len = (xy / radiusSteps).getLength();

            if (len > 1.f)
                continue;

            // Calculate grid coordinates
            auto coord = shapeCenter + xy;

            // Check if coordinates are in range
            if (coord < min || coord >= max)
                continue;

            // Insert coordinates into output iterator
            *out++ = coord;
        }
    }

    return out;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
