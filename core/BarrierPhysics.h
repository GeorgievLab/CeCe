
#ifndef _LIBRARY_BARRIER_PHYSICS_H_
#define _LIBRARY_BARRIER_PHYSICS_H_

/* ************************************************************************ */

// C++
#include <memory>

// Bullet
#include <btBulletDynamicsCommon.h>

// Core
#include "Units.h"

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Cell physics.
 */
class BarrierPhysics
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     * @param x
     * @param y
     */
    BarrierPhysics(World* world, MicroMeters x1, MicroMeters y1, MicroMeters x2, MicroMeters y2);


    /**
     * @brief Destructor.
     */
    virtual ~BarrierPhysics();


public:



public:



// Protected Data Members
protected:

    World* m_world;

    /// Object shape.
    std::unique_ptr<btCollisionShape> m_shape;

    /// Cell body.
    std::unique_ptr<btRigidBody> m_rigidBody;

    /// Cell motion state.
    std::unique_ptr<btDefaultMotionState> m_motionState;

};

/* ************************************************************************ */

#endif // _LIBRARY_BARRIER_PHYSICS_H_
