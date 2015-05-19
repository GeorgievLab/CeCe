
#pragma once

/* ************************************************************************ */

// Simulator
#include "CellBase.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "DrawableCell.hpp"
#endif

#ifdef ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Cell representation.
 */
class Cell : public CellBase
{


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param simulation
     * @param type       Cell type.
     */
    explicit Cell(simulator::Simulation& simulation,
                  simulator::Object::Type type = simulator::Object::Type::Dynamic) noexcept;


// Public Accessors
public:


    /**
     * @brief Returns cell radius.
     *
     * @return
     */
    units::Length getRadius() const noexcept
    {
        return calcSphereRadius(getVolume());
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Returns cell render object.
     *
     * @return
     */
    DrawableCell& getRenderObject() noexcept
    {
        return m_renderObject;
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Returns cell render object.
     *
     * @return
     */
    const DrawableCell& getRenderObject() const noexcept
    {
        return m_renderObject;
    }
#endif


// Public Mutators
public:


// Public Operations
public:


    /**
     * @brief Update cell state.
     *
     * @param dt Time step.
     */
    void update(units::Duration dt) override;


    /**
     * @brief Configure object.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::ConfigurationBase& config,
                   simulator::Simulation& simulation) override;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize object for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context) override;
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render cell.
     *
     * @param context
     */
    void draw(render::Context& context) override;
#endif


// Private Data Members
private:

#if ENABLE_RENDER
    DrawableCell m_renderObject;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

