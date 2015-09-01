/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Vector.hpp"
#include "core/Pair.hpp"
#include "core/DynamicArray.hpp"
#include "render/Buffer.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Render object for drawing lines.
 */
class Lines
{


// Public Types
public:


    /// Point type
    using PointType = Vector<float>;

    /// Line type.
    using LineType = Pair<PointType, PointType>;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     */
    explicit Lines(Context& context)
        : m_buffer(context)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     * @param lines
     */
    Lines(Context& context, const DynamicArray<Pair<PointType, PointType>>& lines)
        : m_buffer(context)
    {
        add(lines);
    }


// Public Mutators
public:


    /**
     * @brief Add new lines.
     *
     * @param lines
     */
    void add(const DynamicArray<Pair<PointType, PointType>>& lines);


    /**
     * @brief Remove all lines.
     */
    void clear()
    {
        m_count = 0;
    }


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param context
     */
    void draw(Context& context) noexcept;


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

    /// Number of lines.
    std::size_t m_count = 0;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
