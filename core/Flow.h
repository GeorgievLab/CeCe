
#ifndef _LIBRARY_FLOW_H_
#define _LIBRARY_FLOW_H_

/* ************************************************************************ */

// C++
#include <cstddef>

// wxWidgets
#include <wx/dc.h>

// Box2D
#include <Box2D/Box2D.h>

// Core
#include "Units.h"
#include "Vect.h"

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Fluild flow representation.
 */
class Flow
{

public:


    /**
     * @brief Constructor.
     *
     * @param world
     * @param x
     * @param y
     * @param vx
     * @param vy
     */
    Flow(World* world, MicroMeters x, MicroMeters y, MicroMeters vx, MicroMeters vy);


    /**
     * @brief Destructor.
     */
    virtual ~Flow();


public:


    /**
     * @brief Render flow.
     *
     * @param dc Device context.
     */
    void Render(wxDC& dc) const noexcept;


private:


    /// Cell world.
    World* m_world;

    /// Flow origin.
    Vect<MicroMeters> m_origin;

    /// Flow velocity and direction.
    Vect<MicroMeters> m_orientation;

};

/* ************************************************************************ */

#endif // _LIBRARY_FLOW_H_
