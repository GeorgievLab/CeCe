
#pragma once

/* ************************************************************************ */

#ifdef THREAD_SAFE
#include <mutex>
#endif

// Simulator
#include "core/Grid.hpp"
#include "simulator/Module.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Grid.hpp"
#include "SignalGridDrawable.hpp"
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
    SignalGrid& getGrid() noexcept
    {
        return m_grid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const SignalGrid& getGrid() const noexcept
    {
        return m_grid;
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    SignalGridDrawable& getDrawable() noexcept
    {
        return m_drawable;
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    const SignalGridDrawable& getDrawable() const noexcept
    {
        return m_drawable;
    }
#endif


    /**
     * @brief Returns diffussion coefficients.
     *
     * @return
     */
    Coefficients getCoefficients() const noexcept
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
    void setCoefficients(Coefficients coefficients) noexcept
    {
        m_coefficients = coefficients;
    }


    /**
     * @brief Change diffussion coefficient.
     *
     * @param pos
     * @param value
     */
    void setCoefficient(unsigned int pos, float value) noexcept
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
     */
    void configure(const simulator::ConfigurationBase& config) override;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize module for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context) override;
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param world
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


// Private Data Members
private:


    /// Diffusion coeffients.
    Coefficients m_coefficients{1.f};

    /// Current signal grid.
    SignalGrid m_grid;

    /// Signal grid used for updating.
    SignalGrid m_gridBack;

#ifdef ENABLE_RENDER
    /// Drawable signal grid
    SignalGridDrawable m_drawable;
#endif

#ifdef THREAD_SAFE
    std::mutex m_mutex;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
