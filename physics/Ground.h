
#ifndef _PHYSICS_GROUND_H_
#define _PHYSICS_GROUND_H_

/* ************************************************************************ */

// Physics
#include "physics/Object.h"

/* ************************************************************************ */

namespace physics {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Ground physical object.
 */
class Ground : public Object
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     */
    explicit Ground(World* world);

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _PHYSICS_GROUND_H_
