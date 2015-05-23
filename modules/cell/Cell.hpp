
#pragma once

/* ************************************************************************ */

// Simulator
#include "CellBase.hpp"

#if ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Object.hpp"
#include "DrawableCell.hpp"
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
    render::ObjectPtr<DrawableCell> m_renderObject;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */

