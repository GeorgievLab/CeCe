
#ifndef _LIBRARY_BARRIER_H_
#define _LIBRARY_BARRIER_H_

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
 * @brief Barrier.
 */
class Barrier
{

public:


    /**
     * @brief Constructor.
     *
     * @param world
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    Barrier(World* world, MicroMeters x1, MicroMeters y1, MicroMeters x2, MicroMeters y2);


    /**
     * @brief Destructor.
     */
    ~Barrier();


    /**
     * @brief Copy constructor.
     */
    Barrier(const Barrier&) = delete;


    /**
     * @brief Move constructor.
     */
    Barrier(Barrier&& rhs) noexcept;


public:


public:


    /**
     * @brief Render flow.
     *
     * @param dc Device context.
     */
    //void Render(wxDC& dc) const noexcept;
    void Render() const noexcept;


private:


    /// Cell world.
    World* m_world;

    /// Physical body.
    b2Body* m_physicsBody = nullptr;

    /// Barrier size.
    Vect<MicroMeters> m_size;

};

/* ************************************************************************ */

#endif // _LIBRARY_BARRIER_H_
