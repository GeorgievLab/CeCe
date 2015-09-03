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
#include "render/Buffer.hpp"
#include "render/GridBase.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief OpenGL grid for vectors object.
 */
class GridVector : public GridBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     * @param size    Grid size.
     * @param data    Vector data.
     * @param max     Maximum value.
     */
    GridVector(Context& context, Size size, const Vector<float>* data, float max = 1);


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     * @param size    Grid size.
     * @param data    Vector data.
     * @param max     Maximum value.
     */
    template<typename T>
    GridVector(Context& context, Size size, const Vector<T>* data, float max = 1)
        : GridVector(context, std::move(size), reinterpret_cast<const Vector<float>*>(data), max)
    {
        static_assert(sizeof(T) == sizeof(float), "T must have same size as float");
    }


// Public Mutators
public:


    /**
     * @brief Set maximum value.
     *
     * @param max
     */
    void setMax(float max) noexcept
    {
        m_max = max;
    }


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param context
     */
    void draw(Context& context) noexcept;


    /**
     * @brief Resize grid.
     *
     * @param size
     * @param data
     */
    void resize(Size size, const Vector<float>* data);


    /**
     * @brief Update data.
     *
     * @param data
     */
    void update(const Vector<float>* data) noexcept;


    /**
     * @brief Update data.
     *
     * @tparam T Vector type.
     *
     * @param data
     */
    template<typename T>
    void update(const Vector<T>* data) noexcept
    {
        static_assert(sizeof(T) == sizeof(float), "T must have same size as float");
        update(reinterpret_cast<const Vector<float>*>(data));
    }


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

    /// Maximum value.
    float m_max = 1;
};

/* ************************************************************************ */

}

/* ************************************************************************ */
