/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Hynek Kasl <hkasl@students.zcu.cz>                               */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <numeric>
#include <iterator>
#include <initializer_list>

// Simulator
#include "core/Real.hpp"
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "core/StaticArray.hpp"

/* ************************************************************************ */

namespace plugin {
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
    using ValueType = RealType;

    /// Direction index type.
    using IndexType = unsigned int;


// Public Constants
public:


    /// Maximum speed in lattice.
    static constexpr ValueType MAX_SPEED = 0.5f;

    /// Number of stored values.
    static constexpr IndexType SIZE = 9;

    /// Direction weights
    static constexpr StaticArray<ValueType, SIZE> DIRECTION_WEIGHTS = {{
        4.f / 9.f, // Center
        1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, // Linear
        1.f / 36.f, 1.f / 36.f, 1.f / 36.f, 1.f / 36.f // Diagonal
    }};

    /// Direction velocities
    static constexpr StaticArray<Vector<int>, SIZE> DIRECTION_VELOCITIES = {{
        { 0,  0},
        { 1,  0}, { 0,  1}, {-1,  0}, { 0, -1},
        { 1,  1}, {-1,  1}, {-1, -1}, { 1, -1}
    }};

    /// Direction opposites
    static constexpr StaticArray<IndexType, SIZE> DIRECTION_OPPOSITES = {{
        0,
        3, 4, 1, 2,
        7, 8, 5, 6
    }};


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param velocity Initial velocity.
     * @param rho      Initial density.
     */
    LatticeData(const Vector<ValueType>& velocity = Zero, ValueType rho = 1.f)
    {
        init(velocity, rho);
    }


    /**
     * @brief Constructor.
     *
     * @param values
     */
    explicit LatticeData(const StaticArray<ValueType, SIZE>& values) noexcept
        : m_values(values)
    {
        // Nothing to do
    }


    /**
     * @brief Copy constructor.
     */
    LatticeData(const LatticeData&) = default;


    /**
     * @brief Move constructor.
     */
    LatticeData(LatticeData&&) = default;


// Public Operators
public:


    /**
     * @brief Returns function value.
     *
     * @param pos
     *
     * @return
     */
    ValueType operator[](IndexType pos) const noexcept
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
    ValueType& operator[](IndexType pos) noexcept
    {
        return m_values[pos];
    }


    /**
     * @brief Assignment operator.
     *
     * @param values
     *
     * @return *this.
     */
    LatticeData& operator=(const StaticArray<ValueType, SIZE>& values) noexcept
    {
        m_values = values;
        return *this;
    }


    /**
     * @brief Copy assignment operator.
     */
    LatticeData& operator=(const LatticeData&) = default;


    /**
     * @brief Move assignment operator.
     */
    LatticeData& operator=(LatticeData&&) = default;


// Public Accessors
public:


    /**
     * @brief Returns if current cell is a static obstacle.
     *
     * @return
     */
    bool isStaticObstacle() const noexcept
    {
        return m_staticObstacle;
    }


    /**
     * @brief Returns if current cell is a dynamic obstacle.
     *
     * @return
     */
    bool isDynamicObstacle() const noexcept
    {
        return m_dynamicObstacle;
    }


    /**
     * @brief Returns if current cell is an obstacle.
     *
     * @return
     */
    bool isObstacle() const noexcept
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
    ValueType get(IndexType pos) const noexcept
    {
        return m_values[pos];
    }


    /**
     * @brief Returns values.
     *
     * @return
     */
    const StaticArray<ValueType, SIZE>& getValues() const noexcept
    {
        return m_values;
    }


// Public Mutators
public:


    /**
     * @brief Set if current cell is a static obstacle.
     *
     * @param flag
     */
    void setStaticObstacle(bool flag) noexcept
    {
        m_staticObstacle = flag;
    }


    /**
     * @brief Set if current cell is a dynamic obstacle.
     *
     * @param flag If cell is dynamic obstacle.
     * @param u    Obstacle velocity.
     */
    void setDynamicObstacle(bool flag, Vector<ValueType> u = Zero) noexcept
    {
        m_dynamicObstacle = flag;
        m_dynamicObstacleVelocity = std::move(u);
    }


    /**
     * @brief Set as inlet.
     *
     * @param v Inlet velocity.
     */
    void inlet(const Vector<ValueType>& v) noexcept;


    /**
     * @brief Set as outlet.
     */
    void outlet() noexcept;


// Public Operations
public:


    /**
     * @brief Clear values.
     */
    void clear() noexcept
    {
        m_values = calcEquilibrium({0, 0}, 1.f);
    }


    /**
     * @brief Calculate Rho, total density.
     *
     * @return
     */
    ValueType calcRho() const noexcept
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
    ValueType sumValues(std::initializer_list<IndexType> list) const noexcept
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
    Vector<ValueType> calcVelocity() const noexcept
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
    Vector<ValueType> calcVelocityNormalized() const noexcept
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
    static StaticArray<ValueType, SIZE> calcEquilibrium(const Vector<ValueType>& u, ValueType rho) noexcept;


// Private Data Members
public:

    /// Distribution functions.
    StaticArray<ValueType, SIZE> m_values;

    /// If current item is a static obstacle.
    bool m_staticObstacle = false;

    /// If current item is a dynamic obstacle.
    bool m_dynamicObstacle = false;

    // Velocity of dynamic obstacle.
    Vector<ValueType> m_dynamicObstacleVelocity = Zero;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
