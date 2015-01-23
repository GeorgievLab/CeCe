
#ifndef _PHYSICS_OBJECT_H_
#define _PHYSICS_OBJECT_H_

/* ************************************************************************ */

// C++
#include <memory>

// Physics
#include "physics/Position.h"

/* ************************************************************************ */

class btCollisionShape;
class btRigidBody;
class btDefaultMotionState;

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Physical object.
 */
class Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     */
    explicit Object(World* world);


    /**
     * @brief Destructor.
     */
    virtual ~Object();


// Public Accessors
public:


    /**
     * @brief Returns physical world.
     */
    World* getWorld() const noexcept
    {
        return m_world;
    }


    /**
     * @brief Returns current position.
     *
     * @return
     */
    Position getPosition() const noexcept;


    /**
     * @brief Returns object shape.
     *
     * @return
     */
    btCollisionShape* getShape() const noexcept
    {
        return m_shape.get();
    }


    /**
     * @brief Returns object's rigid body.
     *
     * @return
     */
    btRigidBody* getRigidBody() const noexcept
    {
        return m_rigidBody.get();
    }


    /**
     * @brief Returns object's rigid body.
     *
     * @return
     */
    btDefaultMotionState* getMotionState() const noexcept
    {
        return m_motionState.get();
    }


// Public Mutators
public:


    /**
     * @brief Change object position.
     *
     * @param pos
     */
    void setPosition(Position pos) noexcept;


// Public Operations
public:


    /**
     * @brief Register rigid body to world.
     */
    void registerRigidBody() noexcept;


    /**
     * @brief Unregister rigid body to world.
     */
    void unregisterRigidBody() noexcept;


// Protected Data Members
protected:

    /// World.
    World * const m_world;

    /// Object shape.
    std::unique_ptr<btCollisionShape> m_shape;

    /// Cell body.
    std::unique_ptr<btRigidBody> m_rigidBody;

    /// Cell motion state.
    std::unique_ptr<btDefaultMotionState> m_motionState;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _PHYSICS_OBJECT_H_
