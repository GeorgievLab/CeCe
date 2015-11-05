/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
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


// Public Enums
public:


    /**
     * @brief Inlet/Outlet direction.
     */
    enum Direction
    {
        DirRight   = 0,
        DirLeft    = 1,
        DirUp      = 2,
        DirDown    = 3,
        DirCount
    };


    /**
     * @brief Cell type.
     */
    enum class Type
    {
        None,
        StaticObstacle,
        DynamicObstacle,
        BGK
    };


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

    /// Direction weight for center.
    static constexpr ValueType WEIGHT_CENTER = 4.0 / 9.0;

    /// Direction weight for linear.
    static constexpr ValueType WEIGHT_LINEAR = 1.0 / 9.0;

    /// Direction weight for diagonal.
    static constexpr ValueType WEIGHT_DIAGONAL = 1.0 / 36.0;

    /// Direction weights
    static constexpr StaticArray<ValueType, SIZE> DIRECTION_WEIGHTS = {{
        WEIGHT_CENTER, // Center
        WEIGHT_DIAGONAL,
        WEIGHT_LINEAR,
        WEIGHT_DIAGONAL,
        WEIGHT_LINEAR,
        WEIGHT_DIAGONAL,
        WEIGHT_LINEAR,
        WEIGHT_DIAGONAL,
        WEIGHT_LINEAR
    }};

    /// Direction velocities
    static constexpr StaticArray<Vector<int>, SIZE> DIRECTION_VELOCITIES = {{
        { 0,  0},
        {-1,  1}, {-1,  0}, {-1, -1}, { 0, -1},
        { 1, -1}, { 1,  0}, { 1,  1}, { 0,  1}
    }};

    /// Direction opposites
    static constexpr StaticArray<IndexType, SIZE> DIRECTION_OPPOSITES = {{
        0,
        5, 6, 7, 8,
        1, 2, 3, 4
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
     * @brief Returns cell type.
     *
     * @return
     */
    Type getType() const noexcept
    {
        return m_type;
    }


    /**
     * @brief Returns if current cell is dummy.
     *
     * @return
     */
    bool isDummy() const noexcept
    {
        return getType() == Type::None;
    }


    /**
     * @brief Returns if current cell is a static obstacle.
     *
     * @return
     */
    bool isStaticObstacle() const noexcept
    {
        return getType() == Type::StaticObstacle;
    }


    /**
     * @brief Returns if current cell is a dynamic obstacle.
     *
     * @return
     */
    bool isDynamicObstacle() const noexcept
    {
        return getType() == Type::DynamicObstacle;
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
     * @brief Set cell type.
     *
     * @param type
     */
    void setType(Type type) noexcept
    {
        m_type = type;
    }


    /**
     * @brief Set if current cell is a static obstacle.
     *
     * @param flag
     */
    void setStaticObstacle() noexcept
    {
        setType(Type::StaticObstacle);
    }


    /**
     * @brief Set if current cell is a dynamic obstacle.
     *
     * @param flag If cell is dynamic obstacle.
     * @param u    Obstacle velocity.
     */
    void setDynamicObstacle(Vector<ValueType> u = Zero) noexcept
    {
        setType(Type::DynamicObstacle);
        m_dynamicObstacleVelocity = std::move(u);
    }


    /**
     * @brief Set as inlet.
     *
     * @param v   Inlet velocity.
     * @param dir Direction.
     */
    void inlet(const Vector<ValueType>& v, Direction dir = DirRight) noexcept;


    /**
     * @brief Set as outlet.
     *
     * @param dir Direction.
     */
    void outlet(Direction dir = DirRight) noexcept;


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
        if (isDummy())
            return {};

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
     * @brief Compute of total sum of given value indices.
     *
     * @tparam N
     *
     * @return
     */
    template<std::size_t N>
    ValueType sumValues(StaticArray<IndexType, N> list) const noexcept
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
        if (isDummy())
            return {};

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
        if (isDummy())
            return {};

        const auto rho = calcRho();

        // Return empty vector
        // TODO: can happend?
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
    void collide(ValueType omega) noexcept;


    /**
     * @brief Collide lattice cell - static obstacle.
     *
     * @param omega Relaxation parameter (1 / tau).
     */
    void collideStatic(ValueType omega) noexcept;


    /**
     * @brief Collide lattice cell - dynamic obstacle.
     *
     * @param omega Relaxation parameter (1 / tau).
     */
    void collideDynamic(ValueType omega) noexcept;


    /**
     * @brief Collide lattice cell - no obstacle.
     *
     * @param omega Relaxation parameter (1 / tau).
     */
    void collideBgk(ValueType omega) noexcept;


    /**
     * @brief Microscopic boundary conditions (Zou/He).
     *
     * @param dir Direction.
     * @param vel Velocity.
     * @param rho Density.
     */
    void microscopicBc(Direction dir, const Vector<ValueType>& vel, ValueType rho) noexcept;


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

    /// Cell type.
    Type m_type = Type::None;

    /// Distribution functions.
    StaticArray<ValueType, SIZE> m_values;

    // Velocity of dynamic obstacle.
    Vector<ValueType> m_dynamicObstacleVelocity = Zero;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
