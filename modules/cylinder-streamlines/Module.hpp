
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"
#include "core/Grid.hpp"
#include "core/compatibility.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

#if ENABLE_RENDER
#include "render/Context.hpp"
#include "render/GridVector.hpp"
#include "render/Object.hpp"
#endif

/* ************************************************************************ */

namespace module {
namespace cylinder_streamlines {

/* ************************************************************************ */

/**
 * @brief Module for cylinder streamlines.
 */
class Module : public simulator::Module
{

// Public Types
public:


    /// Grid type.
    using GridType = Grid<VelocityVector>;

    /// Grid size type.
    using SizeType = GridType::SizeType;


// Public Accessors
public:


    /**
     * @brief Returns grid.
     *
     * @return
     */
    GridType& getGrid() NOEXCEPT
    {
        return m_grid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const GridType& getGrid() const NOEXCEPT
    {
        return m_grid;
    }


    /**
     * @brief Returns flow speed.
     *
     * @return
     */
    float getFlowSpeed() const NOEXCEPT
    {
        return m_flowSpeed;
    }


// Public Mutators
public:


    /**
     * @brief Mark state to be updated.
     */
    void markUpdate() NOEXCEPT
    {
        m_update = true;
    }


    /**
     * @brief Set flow speed.
     *
     * @param speed.
     */
    void setFlowSpeed(float speed) NOEXCEPT
    {
        m_flowSpeed = speed;
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::ConfigurationBase& config, simulator::Simulation& simulation) override;


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


// Private Data Members
private:

    /// Flow speed.
    float m_flowSpeed = 50.f;

    /// Center object.
    simulator::Object* m_object = nullptr;

    /// Velocity grid.
    GridType m_grid;

    /// If streams should be updated
    bool m_update = true;

#if ENABLE_RENDER
    /// Render grid for velocities
    render::ObjectPtr<render::GridVector> m_renderObject;
#endif

#if ENABLE_RENDER
    bool m_drawObject = false;
#endif

#if ENABLE_RENDER
    /// If render object should be updated
    bool m_renderUpdate = false;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
