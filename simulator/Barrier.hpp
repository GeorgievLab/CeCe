
#pragma once

/* ************************************************************************ */

// Physics
#include "physics/Barrier.hpp"

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



// Private Data Members
private:

    /// Cell world.
    World* const m_world;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
