
#ifndef _PHYSICS_CELL_H_
#define _PHYSICS_CELL_H_

/* ************************************************************************ */

// Core
#include "core/Units.h"
#include "core/Vect.h"

// Physics
#include "physics/Object.h"

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Cell physics.
 */
class Cell : public Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world Physical world.
     * @param x     Initial X coordinate (2D).
     * @param y     Initial Y coordinate (2D).
     */
    Cell(World* world, MicroMeters x, MicroMeters y);


// Public Accessors
public:


    /**
     * @brief Returns current position.
     *
     * @return
     */
    Vect<MicroMeters> getPosition() const noexcept;


// Public Operations
public:


    /**
     * @brief Reset cell state.
     */
    virtual void reset() noexcept;


// Protected Operations
protected:


    /**
     * @brief Create sphere body.
     *
     * @param radius Sphere radius.
     */
    void createSphereBody(MicroMeters radius) noexcept;


// Private Data Members
private:


    /// Initial position.
    Vect<MicroMeters> m_initPos;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _PHYSICS_CELL_H_
