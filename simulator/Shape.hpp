/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

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
struct DLL_EXPORT ShapeCircle
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
struct DLL_EXPORT Shape
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
    static Shape makeCircle(units::Length radius, PositionVector center = PositionVector::Zero) NOEXCEPT
    {
        return Shape{ShapeType::Circle, {ShapeCircle{center, radius}}};
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
