
#pragma once

/* ************************************************************************ */

// C++
#include <cmath>

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


// Public Enums
public:


    /**
     * @brief Cell shapes.
     */
    enum class Shape
    {
        Sphere
    };


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world     Physical world.
     * @param shapeType Cell shape.
     */
    Cell(World* world, Shape shapeType);


// Public Accessors
public:


    /**
     * @brief Returns cell shape.
     *
     * @return
     */
    Shape getShapeType() const noexcept
    {
        return m_shapeType;
    }


    /**
     * @brief Get cell volume.
     *
     * @return
     */
    MicroMeters3 getVolume() const noexcept
    {
        return m_volume;
    }


// Public Mutators
public:


    /**
     * @brief Set cell volume.
     *
     * @param volume
     */
    void setVolume(MicroMeters3 volume) noexcept;


// Public Operations
public:


    /**
     * @brief Calculate radius for sphere shapes - from cell volume.
     *
     * @param volume Cell volume.
     *
     * @return Radius.
     */
    static MicroMeters calcSphereRadius(MicroMeters3 volume) noexcept
    {
        // 3th root of ((3 / 4 * pi) * volume)
        return MicroMeters(0.62035f * std::pow(volume.value(), 0.3333333f));
    }


// Protected Operations
protected:


    /**
     * @brief Update (create) sphere body.
     */
    void updateSphereBody() noexcept;


// Private Data Members
private:

    /// Cell shape.
    Shape m_shapeType;

    /// Cell volume.
    MicroMeters3 m_volume = 0_um;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
