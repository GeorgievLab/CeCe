
#ifndef _LIBRARY_CELL_PHYSICS_H_
#define _LIBRARY_CELL_PHYSICS_H_

/* ************************************************************************ */

// C++
#include <memory>

// Bullet
#include <btBulletDynamicsCommon.h>

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Cell physics.
 */
class CellPhysics
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
    CellPhysics(World* world, MicroMeters x, MicroMeters y);


    /**
     * @brief Destructor.
     */
    virtual ~CellPhysics();


public:



public:



// Private Data Members
private:

    /// Cell body
    std::unique_ptr<btRigidBody> m_rigidBody;

};

/* ************************************************************************ */

#endif // _LIBRARY_CELL_PHYSICS_H_
