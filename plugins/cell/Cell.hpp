
#pragma once

/* ************************************************************************ */

// Simulator
#include "CellBase.hpp"

// Simulator
#include "core/compatibility.hpp"

#if ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Object.hpp"
#include "DrawableCell.hpp"
#endif

#if THREAD_SAFE
#include "core/Mutex.hpp"
#endif

/* ************************************************************************ */

namespace plugin {
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
                  simulator::Object::Type type = simulator::Object::Type::Dynamic) NOEXCEPT;


// Public Accessors
public:


    /**
     * @brief Returns cell radius.
     *
     * @return
     */
    units::Length getRadius() const NOEXCEPT
    {
        return calcSphereRadius(getVolume());
    }


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
    void configure(const simulator::Configuration& config,
                   simulator::Simulation& simulation) override;


#if ENABLE_RENDER
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
    render::ObjectSharedPtr<DrawableCell> m_renderObject;
#endif

#if THREAD_SAFE
    /// Access mutex.
    mutable Mutex m_mutex;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

