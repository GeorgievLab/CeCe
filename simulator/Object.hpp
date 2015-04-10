
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
 * @brief Basic simulation object.
 */
class Object
{

// Public Types
public:


    /// Object ID type.
    using IdType = unsigned long;


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
     * @brief Returns object ID.
     *
     * @return
     */
    IdType getId() const noexcept
    {
        return m_id;
    }


// Public Operations
public:


    /**
     * @brief Update object state.
     *
     * @param dt Simulation time step.
     */
    virtual void update(units::Duration dt) = 0;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize object for rendering.
     *
     * @param context
     */
    virtual void drawInit(render::Context& context)
    {
        // Nothing to do
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render object.
     *
     * @param context
     */
    virtual void draw(render::Context& context)
    {
        // Nothing to do
    }
#endif

// Private Data Members
private:

    /// ID generator.
    static IdType s_id;

    /// Object unique ID.
    IdType m_id;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
