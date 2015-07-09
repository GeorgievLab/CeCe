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
    Undefined,
    Circle,
    Rectangle
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
 * @brief Structure for rectangle shape.
 */
struct DLL_EXPORT ShapeRectangle
{
    /// Shape center.
    PositionVector center;

    /// Rectangle size.
    SizeVector size;
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
        ShapeRectangle rectangle;
    };


    /**
     * @brief Default constructor.
     */
    Shape() NOEXCEPT
        : type(ShapeType::Undefined)
        , circle()
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param circle
     */
    Shape(ShapeCircle circle) NOEXCEPT
        : type(ShapeType::Circle)
        , circle(std::move(circle))
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param rectangle
     */
    Shape(ShapeRectangle rectangle) NOEXCEPT
        : type(ShapeType::Rectangle)
        , rectangle(std::move(rectangle))
    {
        // Nothing to do
    }


    /**
     * @brief Create circle shape.
     *
     * @param radius Circle radius.
     * @param center Circle center (position offset).
     *
     * @return
     */
    static Shape makeCircle(units::Length radius, PositionVector center = Zero) NOEXCEPT
    {
        return Shape{ShapeCircle{center, radius}};
    }


    /**
     * @brief Create rectangle shape.
     *
     * @param size   Rectangle size.
     * @param center Rectangle center (position offset).
     *
     * @return
     */
    static Shape makeRectangle(SizeVector size, PositionVector center = Zero) NOEXCEPT
    {
        return Shape{ShapeRectangle{center, size}};
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
