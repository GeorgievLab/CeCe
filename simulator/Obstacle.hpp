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
#include "core/compatibility.hpp"
#include "simulator/Object.hpp"
#include "simulator/Shape.hpp"

#ifdef ENABLE_RENDER
#include "render/Object.hpp"
#include "render/Circle.hpp"
#include "render/Rectangle.hpp"
#endif

#if ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulation;
class Configuration;

/* ************************************************************************ */

/**
 * @brief Physical obstacle.
 */
class DLL_EXPORT Obstacle : public Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation Simulation object reference.
     */
    explicit Obstacle(Simulation& simulation) NOEXCEPT
        : Object(simulation, Type::Static)
    {
        getMutableShapes().resize(1);
    }


// Public Operations
public:


    /**
     * @brief Configure obstacle.
     *
     * @param config
     * @param simulation
     */
    void configure(const Configuration& config, Simulation& simulation) override;


#if ENABLE_RENDER
    /**
     * @brief Render obstacle.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context) override;
#endif


// Protected Operations
protected:


#if ENABLE_PHYSICS
    /**
     * @brief Init obstacle physics shapes.
     */
    void initShapes();
#endif


// Private Data Members
private:

#if ENABLE_PHYSICS
    /// Physical body shapes.
    DynamicArray<UniquePtr<b2Shape>> m_bodyShapes;
#endif

#if ENABLE_RENDER
    render::ObjectSharedPtr<render::Circle> m_drawCircle;
    render::ObjectSharedPtr<render::Rectangle> m_drawRectangle;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */
