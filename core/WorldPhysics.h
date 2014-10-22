
#ifndef _LIBRARY_WORLD_PHYSICS_H_
#define _LIBRARY_WORLD_PHYSICS_H_

/* ************************************************************************ */

// C++
#include <memory>

// Bullet
#include <btBulletDynamicsCommon.h>

/* ************************************************************************ */

/**
 * @brief World physics simulation part.
 */
class WorldPhysics
{

// Public Ctors
public:


    /**
     * @brief Constructor.
     */
    WorldPhysics() noexcept;


    /**
     * @brief Destructor.
     */
    virtual ~WorldPhysics() noexcept;


public:



public:



// Public Operations.
public:


    /**
     * @brief Update world.
     */
    void Update() noexcept;


// Private Data Members
private:

    /// Broadphase.
    std::unique_ptr<btBroadphaseInterface> m_broadphase;

    /// Collision configuration.
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;

    /// Collision dispatcher.
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;

    /// Collision solver.
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;

    /// Dynamic world.
    std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

    /// Ground.
    std::unique_ptr<btCollisionShape> m_groundShape;

    /// Motion state for ground.
    std::unique_ptr<btDefaultMotionState> m_groundMotionState;

    /// Rigid body of the ground.
    std::unique_ptr<btRigidBody> m_groundRigidBody;

};

/* ************************************************************************ */

#endif // _LIBRARY_WORLD_PHYSICS_H_
