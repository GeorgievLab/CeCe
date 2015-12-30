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

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/StaticArray.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Class for storing lattice data and perform LB dynamics.
 *
 * Memory and speedup improvement taken from OpenLB TR1:
 * @link http://optilb.com/openlb/wp-content/uploads/2011/12/olb-tr1.pdf
 */
class LatticeCell
{

// Public Enums
public:


    /**
     * @brief Boundary condition position.
     */
    enum Position
    {
        PositionRight  = 0,
        PositionLeft   = 1,
        PositionTop    = 2,
        PositionBottom = 3,
        PositionCount
    };


    /**
     * @brief Cell dynamics type.
     */
    enum class Dynamics
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

    /// Velocity type.
    using VelocityType = Vector<ValueType>;

    /// Density type.
    using DensityType = ValueType;


// Public Constants
public:


    /// Maximum speed in lattice (c^2 = 1/3, c ~= 0.577).
    static constexpr ValueType MAX_SPEED = 0.5;

    /// Number of stored values.
    static constexpr IndexType SIZE = 9;

    /// Direction index map.
    static constexpr StaticArray<StaticArray<IndexType, 3>, 3> INDEX_MAP{{
        {{1, 8, 7}},
        {{2, 0, 6}},
        {{3, 4, 5}}
    }};

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
        {-1,  1}, {-1,  0}, {-1, -1}, { 0, -1}, { 1, -1}, { 1,  0}, { 1,  1}, { 0,  1}
    }};

    /// Direction opposites
    static constexpr StaticArray<IndexType, SIZE> DIRECTION_OPPOSITES = {{
        0, 5, 6, 7, 8, 1, 2, 3, 4
    }};


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param velocity Initial velocity vector.
     * @param rho      Initial density.
     */
    LatticeCell(const VelocityType& velocity = Zero, DensityType rho = 1.0)
    {
        initEquilibrium(velocity, rho);
    }


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


// Public Accessors
public:


    /**
     * @brief Returns cell dynamics type.
     *
     * @return
     */
    Dynamics getDynamics() const noexcept
    {
        return m_dynamics;
    }


    /**
     * @brief Returns if current cell has no dynamics
     *
     * @return
     */
    bool hasNoDynamics() const noexcept
    {
        return getDynamics() == Dynamics::None;
    }


    /**
     * @brief Returns if current cell has BGK dynamics.
     *
     * @return
     */
    bool hasBgkDynamics() const noexcept
    {
        return getDynamics() == Dynamics::BGK;
    }


    /**
     * @brief Returns if current cell is a static obstacle.
     *
     * @return
     */
    bool hasStaticObstacleDynamics() const noexcept
    {
        return getDynamics() == Dynamics::StaticObstacle;
    }


    /**
     * @brief Returns if current cell is a dynamic obstacle.
     *
     * @return
     */
    bool hasDynamicObstacleDynamics() const noexcept
    {
        return getDynamics() == Dynamics::DynamicObstacle;
    }


    /**
     * @brief Returns if current cell is an obstacle.
     *
     * @return
     */
    bool hasObstacleDynamics() const noexcept
    {
        return hasStaticObstacleDynamics() || hasDynamicObstacleDynamics();
    }


    /**
     * @brief Returns mutable population value.
     *
     * @param pos
     *
     * @return
     */
    ValueType& get(IndexType pos) noexcept
    {
        return m_values[pos];
    }


    /**
     * @brief Returns population value.
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
     * @brief Returns mutable populations.
     *
     * @return
     */
    StaticArray<ValueType, SIZE>& getValues() noexcept
    {
        return m_values;
    }


    /**
     * @brief Returns populations.
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
     * @brief Set cell dynamics.
     *
     * @param dynamics
     */
    void setDynamics(Dynamics dynamics) noexcept
    {
        m_dynamics = dynamics;
    }


    /**
     * @brief Disable cell dynamics.
     */
    void setNoDynamics() noexcept
    {
        setDynamics(Dynamics::None);
    }


    /**
     * @brief Set BGK cell dynamics.
     */
    void setBgkDynamics() noexcept
    {
        setDynamics(Dynamics::BGK);
    }


    /**
     * @brief Set if current cell is a static obstacle.
     */
    void setStaticObstacleDynamics() noexcept
    {
        setDynamics(Dynamics::StaticObstacle);
    }


    /**
     * @brief Set if current cell is a dynamic obstacle.
     *
     * @param velocity Obstacle velocity.
     */
    void setDynamicObstacleDynamics(VelocityType velocity = Zero) noexcept
    {
        setDynamics(Dynamics::DynamicObstacle);
        m_dynamicObstacleVelocity = std::move(velocity);
    }


    /**
     * @brief Init cell velocity for boundary condition.
     *
     * @param velocity Cell velocity.
     * @param position Boundary position.
     */
    void initVelocity(Position position, const VelocityType& velocity) noexcept;


    /**
     * @brief Init cell density for boundary condition.
     *
     * @param position Boundary position.
     * @param rho      Cell density.
     */
    void initDensity(Position position, DensityType rho) noexcept;


    /**
     * @brief Init cell as inlet.
     *
     * @param position Boundary position.
     * @param velocity Inlet velocity vector.
     */
    void initInlet(Position position, const VelocityType& velocity) noexcept;


    /**
     * @brief Init cell as outlet.
     *
     * @param position Boundary position.
     * @param rho Outlet density.
     */
    void initOutlet(Position position, ValueType rho) noexcept;


// Public Operations
public:


    /**
     * @brief Calculate Rho, total density.
     *
     * @return
     */
    DensityType calcRho() const noexcept
    {
        if (hasNoDynamics())
            return 0.0;

        ValueType res{};

        for (auto pop : m_values)
            res += pop;

        return res;
    }


    /**
     * @brief Compute of total sum of given value indices.
     *
     * @tparam N Number of indices.
     *
     * @param list List of indices to sum.
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
    VelocityType calcVelocity() const noexcept;


    /**
     * @brief Calculate velocity vector (normalized).
     *
     * @code calcVelocity() / calcRho()
     *
     * @return
     */
    VelocityType calcVelocityNormalized() const noexcept
    {
        if (hasNoDynamics())
            return Zero;

        const auto rho = calcRho();
        Assert(rho >= 0);
        return calcVelocity() / rho;
    }


    /**
     * @brief Initialize cell by equilibrium values.
     *
     * @param velocity Cell velocity vector.
     * @param rho      Cell density.
     */
    void initEquilibrium(const VelocityType& velocity = Zero, DensityType rho = 1.0) noexcept
    {
        m_values = calcEquilibrium(velocity, rho);
    }


    /**
     * @brief Perform cell collision.
     *
     * @param omega Relaxation parameter (1 / tau).
     */
    void collide(ValueType omega) noexcept;


    /**
     * @brief Perform static obstacle collision.
     *
     * @param omega Relaxation parameter (1 / tau).
     */
    void collideStatic(ValueType omega) noexcept;


    /**
     * @brief Perform dynamic obstacle collision.
     *
     * @param omega Relaxation parameter (1 / tau).
     */
    void collideDynamic(ValueType omega) noexcept;


    /**
     * @brief Perform BGK collision.
     *
     * @param omega Relaxation parameter (1 / tau).
     */
    void collideBgk(ValueType omega) noexcept;


    /**
     * @brief Calculate density for specified velocity.
     *
     * @param position Boundary position.
     * @param velocity Required velocity.
     *
     * @return Calculated density vector for fixed velocity.
     */
    DensityType calcVelocity(Position position, const VelocityType& velocity) const noexcept;


    /**
     * @brief Calculate velocity for specified density for boundary condition.
     *
     * @param position Boundary position.
     * @param rho      Required density.
     *
     * @return Calculated velocity for fixed density.
     */
    VelocityType calcDensity(Position position, DensityType rho) const noexcept;


    /**
     * @brief Apply ZouHe boundary conditions to calculate missing populations.
     *
     * @param position Cell position.
     * @param velocity Cell velocity vector.
     * @param rho      Cell density.
     */
    void applyZouHe(Position position, const VelocityType& velocity, DensityType rho) noexcept;


    /**
     * @brief Calculate equilibrum value.
     *
     * @param i        Population index.
     * @param velocity Velocity vector.
     * @param uSq      Squared velocity vector length.
     * @param rho      Total density.
     *
     * @return
     */
    static ValueType calcEquilibrium(IndexType i, const VelocityType& velocity, ValueType uSq, DensityType rho) noexcept;


    /**
     * @brief Calculate equilibrum values.
     *
     * @param velocity Velocity vector.
     * @param rho      Total density.
     *
     * @return
     */
    static StaticArray<ValueType, SIZE> calcEquilibrium(const VelocityType& velocity, DensityType rho) noexcept;


// Private Data Members
public:

    /// Cell dynamics type.
    Dynamics m_dynamics = Dynamics::None;

    /// Distribution functions.
    StaticArray<ValueType, SIZE> m_values;

    // Velocity of dynamic obstacle.
    VelocityType m_dynamicObstacleVelocity = Zero;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
