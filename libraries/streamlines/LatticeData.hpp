
#pragma once

/* ************************************************************************ */

// C++
#include <algorithm>

// Simulator
#include "core/compatibility.hpp"
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/StaticArray.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Class for storing lattice data.
 */
class LatticeData
{


// Public Types
public:


    /// Computation value type.
    using ValueType = float;

    /// Direction index type.
    using IndexType = unsigned int;


// Public Constants
public:


    /// Maximum speed in lattice.
    static CONSTEXPR_CONST ValueType MAX_SPEED = 0.5f;

    /// Number of stored values.
    static CONSTEXPR_CONST IndexType SIZE = 9;

    /// Direction weights
    static CONSTEXPR_CONST StaticArray<ValueType, SIZE> DIRECTION_WEIGHTS = {{
        4.f / 9.f, // Center
        1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, // Linear
        1.f / 36.f, 1.f / 36.f, 1.f / 36.f, 1.f / 36.f // Diagonal
    }};

    /// Direction velocities
    static CONSTEXPR_CONST StaticArray<Vector<int>, SIZE> DIRECTION_VELOCITIES = {{
        { 0,  0},
        { 1,  0}, { 0,  1}, {-1,  0}, { 0, -1},
        { 1,  1}, {-1,  1}, {-1, -1}, { 1, -1}
    }};

    /// Direction opposites
    static CONSTEXPR_CONST StaticArray<IndexType, SIZE> DIRECTION_OPPOSITES = {
        0,
        3, 4, 1, 2,
        7, 8, 5, 6
    };


// Public Operators
public:


    /**
     * @brief Returns function value.
     *
     * @param pos
     *
     * @return
     */
    ValueType operator[](IndexType pos) const NOEXCEPT
    {
        return m_values[pos];
    }


    /**
     * @brief Returns function value.
     *
     * @param pos
     *
     * @return
     */
    ValueType& operator[](IndexType pos) NOEXCEPT
    {
        return m_values[pos];
    }


// Public Accessors
public:


    /**
     * @brief Returns if current cell is a static obstacle.
     *
     * @return
     */
    bool isStaticObstacle() const NOEXCEPT
    {
        return m_staticObstacle;
    }


    /**
     * @brief Returns if current cell is a dynamic obstacle.
     *
     * @return
     */
    bool isDynamicObstacle() const NOEXCEPT
    {
        return m_dynamicObstacle;
    }


    /**
     * @brief Returns if current cell is an obstacle.
     *
     * @return
     */
    bool isObstacle() const NOEXCEPT
    {
        return isStaticObstacle() || isDynamicObstacle();
    }


    /**
     * @brief Returns function value.
     *
     * @param pos
     *
     * @return
     */
    ValueType get(IndexType pos) const NOEXCEPT
    {
        return m_values[pos];
    }


// Public Mutators
public:


    /**
     * @brief Set if current cell is a static obstacle.
     *
     * @param flag
     */
    void setStaticObstacle(bool flag) NOEXCEPT
    {
        m_staticObstacle = flag;
    }


    /**
     * @brief Set if current cell is a dynamic obstacle.
     *
     * @param flag If cell is dynamic obstacle.
     * @param u    Obstacle velocity.
     */
    void setDynamicObstacle(bool flag, Vector<ValueType> u = Vector<ValueType>::Zero) NOEXCEPT
    {
        m_dynamicObstacle = flag;
        m_dynamicObstacleVelocity = std::move(u);
    }


// Public Operations
public:


    /**
     * @brief Clear values.
     */
    void clear() NOEXCEPT
    {
        m_values = calcEquilibrium({0, 0}, 1.f);
        //for (auto& val : m_values)
        //    val = 0.f;
    }


    /**
     * @brief Calculate Rho, total density.
     *
     * @return
     */
    ValueType calcRho() const NOEXCEPT
    {
        using std::begin;
        using std::end;
        return std::accumulate(begin(m_values), end(m_values), ValueType{});
    }


    /**
     * @brief Compute of total sum of given value indices.
     *
     * @return
     */
    ValueType sumValues(std::initializer_list<IndexType> list) const NOEXCEPT
    {
        ValueType res{};

        for (auto i : list)
            res += m_values[i];

        return res;
    }


    /**
     * @brief Calculate velocity vector.
     *
     * @return
     */
    Vector<ValueType> calcVelocity() const NOEXCEPT
    {
        return Vector<ValueType>{
            sumValues({1, 5, 8}) - sumValues({3, 6, 7}),
            sumValues({2, 5, 6}) - sumValues({4, 7, 8})
        };
    }


    /**
     * @brief Calculate velocity vector (normalized).
     *
     * @return
     */
    Vector<ValueType> calcVelocityNormalized() const NOEXCEPT
    {
        const auto rho = calcRho();

        // Return empty vector
        if (rho == 0)
            return Vector<ValueType>{};

        return calcVelocity() / rho;
    }


    /**
     * @brief Initialize cell by velocity.
     *
     * @param velocity
     * @param rho
     */
    void init(const Vector<ValueType>& velocity, ValueType rho = 1.f)
    {
        m_values = calcEquilibrium(velocity, rho);
    }


    /**
     * @brief Collide lattice cell.
     *
     * @param omega Relaxation parameter (1 / tau).
     */
    void collide(ValueType omega);


    /**
     * @brief Calculate equilibrum values.
     *
     * @param u   Velocity vector.
     * @param rho Total density.
     *
     * @return
     */
    static StaticArray<ValueType, SIZE> calcEquilibrium(const Vector<ValueType>& u, ValueType rho) NOEXCEPT;


// Private Data Members
public:

    /// Distribution functions.
    StaticArray<ValueType, SIZE> m_values;

    /// If current item is a static obstacle.
    bool m_staticObstacle = false;

    /// If current item is a dynamic obstacle.
    bool m_dynamicObstacle = false;

    // Velocity of dynamic obstacle.
    Vector<ValueType> m_dynamicObstacleVelocity{Vector<ValueType>::Zero};
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
