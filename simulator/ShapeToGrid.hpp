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
#include <type_traits>

// Simulator
#include "core/compatibility.hpp"
#include "core/Vector.hpp"
#include "simulator/Shape.hpp"

/* ************************************************************************ */

namespace simulator {

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
    const Vector<T>& center, const Vector<T>& max,
    const Vector<T>& min = {})
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
    const Vector<T>& center, const Vector<T>& max,
    const Vector<T>& min = {})
{
    // Get signed type
    using Ts = typename std::make_signed<T>::type;

    // Radius steps in grid
    const auto radiusSteps = Vector<float>(shape.radius) / steps;
    const auto shapeCenter = Vector<Ts>(center + shape.center / steps);
    const auto maxS = Vector<Ts>(max);
    const auto minS = Vector<Ts>(min);

    // Foreach grid given by radius steps
    for (Ts x = Ts(-radiusSteps.getX()); x < Ts(radiusSteps.getX()); ++x)
    {
        for (Ts y = Ts(-radiusSteps.getY()); y < Ts(radiusSteps.getY()); ++y)
        {
            // Calculate normalized length for ellipse
            const Vector<float> xyVec{ static_cast<float>(x), static_cast<float>(y) };
            const Vector<float> lenVec = operator/(xyVec, radiusSteps);
            const float len = lenVec.getLengthSquared();

            if (len > 1.f)
                continue;

            // Calculate grid coordinates
            const Vector<Ts> coord = shapeCenter + Vector<Ts>(xyVec);

            // Check if coordinates are in range
            if (!coord.inRange(minS, maxS))
                continue;

            // Insert coordinates into output iterator
            *out++ = Vector<T>(coord);
        }
    }

    return out;
}

/* ************************************************************************ */

/**
 * @brief Map shape border to grid.
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
template<typename OutIt, typename T, typename StepT, typename OffT = int>
OutIt mapShapeBorderToGrid(OutIt out, const Shape& shape, const Vector<StepT>& steps,
    const Vector<T>& center, const Vector<T>& max, const Vector<T>& min = {},
    OffT off = OffT{})
{
    switch (shape.type)
    {
    default:
        break;

    case ShapeType::Circle:
        return mapShapeBorderToGrid(out, shape.circle, steps, center, max, min, off);
    }

    return out;
}

/* ************************************************************************ */

/**
 * @brief Map circle shape border to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 * @tparam OffT  Radius offset type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 * @param off    Radius offset.
 *
 * @see https://web.archive.org/web/20120225095359/http://homepage.smc.edu/kennedy_john/belipse.pdf
 */
template<typename OutIt, typename T, typename StepT, typename OffT = int>
OutIt mapShapeBorderToGrid(OutIt out, const ShapeCircle& shape, const Vector<StepT>& steps,
    const Vector<T>& center, const Vector<T>& max, const Vector<T>& min = {},
    OffT off = OffT{})
{
    // Get signed type
    using Ts = typename std::make_signed<T>::type;

    // Radius steps in grid
    const Vector<T> radius = shape.radius / steps + off;
    const Vector<Ts> shapeCenter = center + shape.center / steps;
    const Vector<Ts> maxS = max;
    const Vector<Ts> minS = min;

    auto putCoord = [&out, &shapeCenter, &minS, &maxS](Vector<Ts> xy) {
        // Calculate grid coordinates
        const Vector<T> coord = shapeCenter + xy;

        // Check if coordinates are in range
        if (coord.inRange(minS, maxS))
            *out++ = coord;
    };

    const auto radiusSquare = radius * radius;
    const auto twoSquare = 2 * radiusSquare;

    Vector<Ts> xy(radius.getX(), 0);
    Vector<Ts> change(
        radiusSquare.getY() * (1 - 2 * radius.getX()),
        radiusSquare.getX()
    );

    Ts error = 0;
    Vector<Ts> stopping(twoSquare.getY() * radius.getX(), 0);

    while (stopping.getX() >= stopping.getY())
    {
        putCoord(xy);
        putCoord(xy * Vector<Ts>(-1,  1));
        putCoord(xy * Vector<Ts>(-1, -1));
        putCoord(xy * Vector<Ts>( 1, -1));

        ++xy.y();
        stopping.y() += twoSquare.getX();
        error += change.getY();
        change.y() += twoSquare.getX();

        if ((2 * error + change.getX()) > 0)
        {
            --xy.x();
            stopping.x() -= twoSquare.getY();
            error += change.getX();
            change.x() += twoSquare.getY();
        }
    }

    xy = Vector<Ts>(0, radius.getY());
    change = Vector<Ts>(
        radiusSquare.getY(),
        radiusSquare.getX() * (1 - 2 * radius.getY())
    );

    error = 0;
    stopping = Vector<Ts>(0, twoSquare.getY() * radius.getY());

    while (stopping.getX() <= stopping.getY())
    {
        putCoord(xy);
        putCoord(xy * Vector<Ts>(-1,  1));
        putCoord(xy * Vector<Ts>(-1, -1));
        putCoord(xy * Vector<Ts>( 1, -1));

        ++xy.x();
        stopping.x() += twoSquare.getY();
        error += change.getX();
        change.x() += twoSquare.getY();

        if ((2 * error + change.getY()) > 0)
        {
            --xy.y();
            stopping.y() -= twoSquare.getX();
            error += change.getY();
            change.y() += twoSquare.getX();
        }
    }

    return out;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
