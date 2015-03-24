
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Simulation object.
 */
class Object
{

// Public Types
public:


    /// Cell ID type.
    using Id = unsigned long;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Object();


    /**
     * @brief Destructor.
     */
    virtual ~Object();


// Public Accessors
public:


    /**
     * @brief Returns cell ID.
     *
     * @return
     */
    Id getId() const noexcept
    {
        return m_id;
    }


// Public Operations
public:


    /**
     * @brief Update object state.
     *
     * @param dt Time step.
     */
    virtual void update(Duration dt) = 0;

#ifdef ENABLE_RENDER

    /**
     * @brief Render object.
     *
     * @param context
     */
    virtual void render(render::Context& context)
    {
        // Nothing to do
    }

#endif

// Private Data Members
private:

    /// ID generator.
    static Id s_id;

    /// Cell ID
    Id m_id;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
