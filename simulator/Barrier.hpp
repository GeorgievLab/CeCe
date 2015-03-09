
#pragma once

/* ************************************************************************ */

// Physics
#include "physics/Barrier.hpp"

#ifdef ENABLE_RENDER
// Render
#include "render/Context.h"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Barrier representation.
 */
class Barrier : public physics::Barrier
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     */
    explicit Barrier(World* world);


    /**
     * @brief Destructor.
     */
    virtual ~Barrier();


// Public Accessors
public:


    /**
     * @brief Returns world.
     *
     * @return
     */
    World* getWorld() const noexcept
    {
        return m_world;
    }


// Public Mutators
public:


// Public Operations
public:


#ifdef ENABLE_RENDER

    /**
     * @brief Render barrier.
     *
     * @param context
     */
    virtual void draw(render::Context& context);

#endif


// Private Data Members
private:

    /// Cell world.
    World* const m_world;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
