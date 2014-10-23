
#ifndef _SIMULATOR_YEAST_H_
#define _SIMULATOR_YEAST_H_

/* ************************************************************************ */

// C++
#include <cmath>

// Core
#include "core/Units.h"

// Simulator
#include "simulator/Cell.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Yeast representation.
 */
class Yeast : public Cell
{

// Public Ctors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     * @param x
     * @param y
     * @param radius
     */
    Yeast(World* world, MicroMeters x, MicroMeters y, MicroMeters radius = 1_um);


    /**
     * @brief Constructor.
     *
     * @param world
     * @param x
     * @param y
     * @param volume
     */
    Yeast(World* world, MicroMeters x, MicroMeters y, MicroMeters3 volume = 1_um3);


// Public Accessors
public:


    /**
     * @brief Get yeast volume.
     *
     * @return
     */
    MicroMeters3 getVolume() const noexcept
    {
        return calcVolume(m_radius);
    }


    /**
     * @brief Get yeast radius.
     *
     * @return
     */
    MicroMeters getRadius() const noexcept
    {
        return m_radius;
    }


// Public Operations
public:


    /**
     * @brief Render the yeast.
     *
     * @param context
     */
    void draw(render::Context& context) const noexcept override;


    /**
     * @brief Calculate radius from volume.
     *
     * @param volume Volume.
     *
     * @return Radius.
     */
    static MicroMeters calcRadius(MicroMeters3 volume) noexcept
    {
        // 3th root of ((3 / 4 * pi) * volume)
        return MicroMeters(0.62035f * std::pow(volume.value(), 0.3333333f));
    }


    /**
     * @brief Calculate volume from radius.
     *
     * @param radius Radius.
     *
     * @return Volume.
     */
    static MicroMeters3 calcVolume(MicroMeters radius) noexcept
    {
        return MicroMeters3(4.188790205f * radius.value() * radius.value() * radius.value());
    }


// Private Data Members
private:

    /// Yeast radius.
    MicroMeters m_radius;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif // _SIMULATOR_YEAST_H_
