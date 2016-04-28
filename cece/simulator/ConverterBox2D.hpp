/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

#ifndef CECE_ENABLE_BOX2D_PHYSICS
#error Only included for Box2D
#endif

/* ************************************************************************ */

// Box2D
#include <Box2D/Box2D.h>

// CeCe
#include "cece/export.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/UnitsCtors.hpp"
#include "cece/core/VectorUnits.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Simulation units <-> physics units converter.
 *
 * Used physics engine library can be tuned to specific range of values that
 * may not match simulation values, so this class is responsible to convert
 * values between those two systems.
 */
class CECE_EXPORT ConverterBox2D
{

// Public Accessors
public:


    /**
     * @brief Returns simulation time step.
     *
     * @return
     */
    units::Time getTimeStep() const noexcept
    {
        return m_timeStep;
    }


    /**
     * @brief Returns Box2D time step.
     *
     * @return
     */
    units::Time getTimeStepBox2D() const noexcept
    {
        return m_timeStepBox2D;
    }


    /**
     * @brief Returns time coefficient.
     *
     * @return
     */
    RealType getTimeCoefficient() const noexcept;


    /**
     * @brief Returns length coefficient.
     *
     * @return
     */
    RealType getLengthCoefficient() const noexcept
    {
        return m_lengthCoefficient;
    }


// Public Mutators
public:


    /**
     * @brief Change simulation time step.
     *
     * @param timeStep
     */
    void setTimeStep(units::Time timeStep) noexcept
    {
        m_timeStep = timeStep;
    }


    /**
     * @brief Set Box2D time step.
     *
     * @param timeStep
     */
    void setTimeStepBox2D(units::Time timeStep) noexcept
    {
        m_timeStepBox2D = timeStep;
    }


    /**
     * @brief Set length coefficient.
     *
     * @param value
     */
    void setLengthCoefficient(RealType value) noexcept
    {
        m_lengthCoefficient = value;
    }


// Public Operations
public:


    /**
     * @brief Convert length.
     *
     * @param length
     *
     * @return
     */
    units::Length convertLength(float32 length) const noexcept;


    /**
     * @brief Convert length.
     *
     * @param length
     *
     * @return
     */
    float32 convertLength(units::Length length) const noexcept;


    /**
     * @brief Convert position.
     *
     * @param position
     *
     * @return
     */
    PositionVector convertPosition(b2Vec2 position) const noexcept;


    /**
     * @brief Convert position.
     *
     * @param position
     *
     * @return
     */
    b2Vec2 convertPosition(PositionVector position) const noexcept;


    /**
     * @brief Convert rotation.
     *
     * @param angle
     *
     * @return
     */
    units::Angle convertAngle(float32 angle) const noexcept;


    /**
     * @brief Convert rotation.
     *
     * @param angle
     *
     * @return
     */
    float32 convertAngle(units::Angle angle) const noexcept;


    /**
     * @brief Convert linear velocity.
     *
     * @param velocity
     *
     * @return
     */
    VelocityVector convertLinearVelocity(b2Vec2 velocity) const noexcept;


    /**
     * @brief Convert linear velocity.
     *
     * @param velocity
     *
     * @return
     */
    b2Vec2 convertLinearVelocity(VelocityVector velocity) const noexcept;


    /**
     * @brief Convert angular velocity.
     *
     * @param velocity
     *
     * @return
     */
    units::AngularVelocity convertAngularVelocity(float32 velocity) const noexcept;


    /**
     * @brief Convert angular velocity.
     *
     * @param velocity
     *
     * @return
     */
    float32 convertAngularVelocity(units::AngularVelocity velocity) const noexcept;


    /**
     * @brief Convert linear acceleration.
     *
     * @param acceleration
     *
     * @return
     */
    AccelerationVector convertLinearAcceleration(b2Vec2 acceleration) const noexcept;


    /**
     * @brief Convert linear acceleration.
     *
     * @param acceleration
     *
     * @return
     */
    b2Vec2 convertLinearAcceleration(AccelerationVector acceleration) const noexcept;


    /**
     * @brief Convert angular acceleration.
     *
     * @param acceleration
     *
     * @return
     */
    units::Acceleration convertAngularAcceleration(float32 acceleration) const noexcept;


    /**
     * @brief Convert angular acceleration.
     *
     * @param acceleration
     *
     * @return
     */
    float32 convertAngularAcceleration(units::Acceleration acceleration) const noexcept;


    /**
     * @brief Convert force.
     *
     * @param force
     *
     * @return
     */
    ForceVector convertForce(b2Vec2 force) const noexcept;


    /**
     * @brief Convert force.
     *
     * @param force
     *
     * @return
     */
    b2Vec2 convertForce(ForceVector force) const noexcept;


    /**
     * @brief Convert torque.
     *
     * @param torque
     *
     * @return
     */
    units::Force convertTorque(float32 torque) const noexcept;


    /**
     * @brief Convert torque.
     *
     * @param torque
     *
     * @return
     */
    float32 convertTorque(units::Force torque) const noexcept;


    /**
     * @brief Convert impulse.
     *
     * @param impulse
     *
     * @return
     */
    ImpulseVector convertLinearImpulse(b2Vec2 impulse) const noexcept;


    /**
     * @brief Convert impulse.
     *
     * @param force
     *
     * @return
     */
    b2Vec2 convertLinearImpulse(ImpulseVector impulse) const noexcept;


    /**
     * @brief Convert impulse.
     *
     * @param impulse
     *
     * @return
     */
    units::Impulse convertAngularImpulse(float32 impulse) const noexcept;


    /**
     * @brief Convert impulse.
     *
     * @param impulse
     *
     * @return
     */
    float32 convertAngularImpulse(units::Impulse impulse) const noexcept;


    /**
     * @brief Convert mass.
     *
     * @param mass
     *
     * @return
     */
    units::Mass convertMass(float32 mass) const noexcept;


    /**
     * @brief Convert mass.
     *
     * @param mass
     *
     * @return
     */
    float32 convertMass(units::Mass mass) const noexcept;


    /**
     * @brief Convert density.
     *
     * @param density
     *
     * @return
     */
    units::Density convertDensity(float32 density) const noexcept;


    /**
     * @brief Convert density.
     *
     * @param density
     *
     * @return
     */
    float32 convertDensity(units::Density density) const noexcept;


    /**
     * @brief Returns maximum translation distance per iteration.
     *
     * @return
     */
    units::Length getMaxObjectTranslation() const noexcept;


    /**
     * @brief Returns converter instance.
     * @return
     */
    static ConverterBox2D& getInstance() noexcept;


// Private Data Members
private:

    /// Box2D time step.
    units::Time m_timeStepBox2D = units::s(1.0 / 60.0);

    /// Simulation time step
    units::Time m_timeStep;

    /// Length coefficient.
    RealType m_lengthCoefficient = 1;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
