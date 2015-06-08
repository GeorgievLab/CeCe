
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/Grid.hpp"
#include "simulator/Module.hpp"

#if ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Object.hpp"
#include "render/GridColorSmooth.hpp"
#endif

// Module
#include "Signal.hpp"
#include "SignalGrid.hpp"

/* ************************************************************************ */

namespace module {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Module for diffusion.
 */
class Module : public simulator::Module
{

// Public Types
public:


    /// Coefficients type.
    using Coefficients = Signal;


// Public Ctors & Dtors
public:


    /**
    * @brief Constructor.
    */
    Module();


    /**
     * @brief Destructor.
     */
    virtual ~Module();


// Public Accessors
public:


    /**
     * @brief Returns grid.
     *
     * @return
     */
    SignalGrid& getGrid() NOEXCEPT
    {
        return m_grid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const SignalGrid& getGrid() const NOEXCEPT
    {
        return m_grid;
    }


    /**
     * @brief Returns diffussion coefficients.
     *
     * @return
     */
    Coefficients getCoefficients() const NOEXCEPT
    {
        return m_coefficients;
    }


// Public Mutators
public:


    /**
     * @brief Resize signal grid.
     *
     * @param size New grid size.
     */
    void setSize(const Vector<SignalGrid::SizeType>& size)
    {
        m_grid.resize(size);
        m_gridBack.resize(size);
    }


    /**
     * @brief Change diffussion coefficients.
     *
     * @param coefficients
     */
    void setCoefficients(Coefficients coefficients) NOEXCEPT
    {
        m_coefficients = coefficients;
    }


    /**
     * @brief Change diffussion coefficient.
     *
     * @param pos
     * @param value
     */
    void setCoefficient(unsigned int pos, float value) NOEXCEPT
    {
        m_coefficients[pos] = value;
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt    Simulation time step.
     * @param world World object.
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


#if ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


#if ENABLE_RENDER
    /**
     * @brief Update drawable.
     */
    void updateDrawable();
#endif


// Private Data Members
private:


    /// Diffusion coeffients.
    Coefficients m_coefficients{1.f};

    /// Current signal grid.
    SignalGrid m_grid;

    /// Signal grid used for updating.
    SignalGrid m_gridBack;

    /// Background color.
    render::Color m_background = render::colors::BLACK;

    /// Signal colors.
    std::vector<render::Color> m_colors;

#if ENABLE_RENDER
    /// Drawable signal grid.
    render::ObjectPtr<render::GridColorSmooth> m_drawable;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
