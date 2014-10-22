
#ifndef _LIBRARY_YEAST_H_
#define _LIBRARY_YEAST_H_

/* ************************************************************************ */

// C++
#include <cmath>

// Library
#include "Cell.h"
#include "Units.h"

/* ************************************************************************ */

/**
 * @brief Yeast representation.
 */
class Yeast : public Cell
{

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


public:


    /**
     * @brief Get yeast volume.
     *
     * @return
     */
    MicroMeters3 GetVolume() const noexcept
    {
        return m_volume;
    }


    /**
     * @brief Get yeast radius.
     *
     * @return
     */
    MicroMeters GetRadius() const noexcept
    {
        return CalcRadius(m_volume);
    }


public:


public:


    /**
     * @brief Render the cell.
     */
    void Render() const noexcept override;


    /**
     * @brief Calculate radius from volume.
     *
     * @param volume Volume.
     *
     * @return Radius.
     */
    static MicroMeters CalcRadius(MicroMeters3 volume) noexcept
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
    static MicroMeters3 CalcVolume(MicroMeters radius) noexcept
    {
        return MicroMeters3(4.188790205f * radius.value() * radius.value() * radius.value());
    }


private:

    /// Yeast volume.
    MicroMeters3 m_volume;

};

/* ************************************************************************ */

#endif // _LIBRARY_YEAST_H_
