/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "core/Units.hpp"
#include "core/Variant.hpp"
#include "core/VectorUnits.hpp"
#include "core/DynamicArray.hpp"

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
    Undefined   = 0,
    Circle      = 1,
    Rectangle   = 2,
    Edges       = 3
};

/* ************************************************************************ */

/**
 * @brief Structure for circle shape.
 */
struct ShapeUndefined {};

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
 * @brief Structure for rectangle shape.
 */
struct ShapeRectangle
{
    /// Shape center.
    PositionVector center;

    /// Rectangle size.
    SizeVector size;
};

/* ************************************************************************ */

/**
 * @brief Structure for edge chain shape.
 */
struct ShapeEdges
{
    /// Shape center.
    PositionVector center;

    /// List of edges.
    DynamicArray<PositionVector> edges;
};

/* ************************************************************************ */

/**
 * @brief Structure for storing shape.
 */
class Shape : public Variant<
    ShapeUndefined,
    ShapeCircle,
    ShapeRectangle,
    ShapeEdges
>
{

    /// Parent type
    using Parent = Variant<
        ShapeUndefined,
        ShapeCircle,
        ShapeRectangle,
        ShapeEdges
    >;


// Public Structures
public:


    struct GetType : public VariantStaticVisitor<ShapeType>
    {
        ShapeType operator()(ShapeUndefined) const noexcept { return ShapeType::Undefined; }
        ShapeType operator()(const ShapeCircle&) const noexcept { return ShapeType::Circle; }
        ShapeType operator()(const ShapeRectangle&) const noexcept { return ShapeType::Rectangle; }
        ShapeType operator()(const ShapeEdges&) const noexcept { return ShapeType::Edges; }
    };

// Public Ctors & Dtors
public:


    /**
     * @brief Parent's ctors.
     */
    using Parent::Parent;


// Public Accessors
public:


    /**
     * @brief Get shape type.
     *
     * @return
     */
    ShapeType getType() const noexcept
    {
        return variantApplyVisitor(GetType{}, *this);
    }


// Public Operations
public:


    /**
     * @brief Create undefined shape.
     *
     * @return
     */
    static Shape makeUndefined() noexcept
    {
        return Shape{ShapeUndefined{}};
    }


    /**
     * @brief Create circle shape.
     *
     * @param radius Circle radius.
     * @param center Circle center (position offset).
     *
     * @return
     */
    static Shape makeCircle(units::Length radius, PositionVector center = Zero) noexcept
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
    static Shape makeRectangle(SizeVector size, PositionVector center = Zero) noexcept
    {
        return Shape{ShapeRectangle{center, size}};
    }


    /**
     * @brief Create edges shape.
     *
     * @param edges  List of edges.
     * @param center Shape center (position offset).
     *
     * @return
     */
    static Shape makeEdges(DynamicArray<PositionVector> edges, PositionVector center = Zero) noexcept
    {
        return Shape{ShapeEdges{center, std::move(edges)}};
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
