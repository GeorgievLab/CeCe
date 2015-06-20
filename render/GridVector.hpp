/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
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
class DLL_EXPORT GridVector : public GridBase
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     * @param size    Grid size.
     * @param data    Vector data.
     */
    GridVector(Context& context, Size size, const Vector<float>* data);


    /**
     * @brief Constructor.
     *
     * @param context Render context.
     * @param size    Grid size.
     * @param data    Vector data.
     */
    template<typename T>
    GridVector(Context& context, Size size, const Vector<T>* data)
        : GridVector(context, std::move(size), reinterpret_cast<const Vector<float>*>(data))
    {
        static_assert(sizeof(T) == sizeof(float), "T must have same size as float");
    }


// Public Operations
public:


    /**
     * @brief Render at current position.
     *
     * @param context
     */
    void draw(Context& context) NOEXCEPT;


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
    void update(const Vector<float>* data) NOEXCEPT;


    /**
     * @brief Update data.
     *
     * @tparam T Vector type.
     *
     * @param data
     */
    template<typename T>
    void update(const Vector<T>* data) NOEXCEPT
    {
        static_assert(sizeof(T) == sizeof(float), "T must have same size as float");
        update(reinterpret_cast<const Vector<float>*>(data));
    }


// Private Data Members
private:

    /// Buffer object.
    Buffer m_buffer;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
