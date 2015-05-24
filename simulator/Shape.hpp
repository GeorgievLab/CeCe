
#pragma once

/* ************************************************************************ */

// C++
#include <cassert>

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"

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
    core::PositionVector center;

    /// Circle radius.
	core::units::Length radius;
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
	static Shape makeCircle(core::units::Length radius, core::PositionVector center = { 0, 0 }) NOEXCEPT
    {
        return Shape{ShapeType::Circle, ShapeCircle{center, radius}};
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
