
#ifndef _PHYSICS_WORLD_H_
#define _PHYSICS_WORLD_H_

/* ************************************************************************ */

// C++
#include <memory>
#include <array>

/* ************************************************************************ */

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

class Ground;

/* ************************************************************************ */

/**
 * @brief World physics.
 */
class World
{

// Public Ctors
public:


    /**
     * @brief Constructor.
     */
    World() noexcept;


    /**
     * @brief Destructor.
     */
    virtual ~World() noexcept;


// Public Accessors
public:


    /**
     * @brief Returns dynamics world.
     *
     * @return
     */
    btDiscreteDynamicsWorld* getDynamicsWorld() const noexcept
    {
        return m_dynamicsWorld.get();
    }


    /**
     * @brief Returns ground object.
     *
     * @return
     */
    Ground* getGround() const noexcept
    {
        return m_ground.get();
    }


    /**
     * @brief Returns gravity.
     *
     * @return
     */
    std::array<float, 3> getGravity() const noexcept;


// Public Operations
public:


    /**
     * @brief Update world.
     *
     * @param step
     */
    void update(float step) noexcept;


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

    /// Ground object.
    std::unique_ptr<Ground> m_ground;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _PHYSICS_WORLD_H_
