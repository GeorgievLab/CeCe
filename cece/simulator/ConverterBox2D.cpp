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

// Declaration
#include "cece/simulator/ConverterBox2D.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

RealType ConverterBox2D::getTimeCoefficient() const noexcept
{
    return m_timeStepBox2D / m_timeStep;
}

/* ************************************************************************ */

units::Length ConverterBox2D::convertLength(float32 length) const noexcept
{
    const auto coeff = 1.0 / getLengthCoefficient();

    return coeff * units::Length(length);
}

/* ************************************************************************ */

float32 ConverterBox2D::convertLength(units::Length length) const noexcept
{
    const auto coeff = getLengthCoefficient();

    return coeff * length.value();
}

/* ************************************************************************ */

PositionVector ConverterBox2D::convertPosition(b2Vec2 position) const noexcept
{
    const auto coeff = 1.0 / getLengthCoefficient();

    return coeff * PositionVector{
        units::Length(position.x),
        units::Length(position.y)
    };
}

/* ************************************************************************ */

b2Vec2 ConverterBox2D::convertPosition(PositionVector position) const noexcept
{
    const auto coeff = getLengthCoefficient();

    return coeff * b2Vec2{
        static_cast<float32>(position.getX().value()),
        static_cast<float32>(position.getY().value())
    };
}

/* ************************************************************************ */

units::Angle ConverterBox2D::convertAngle(float32 angle) const noexcept
{
    return units::Angle{angle};
}

/* ************************************************************************ */

float32 ConverterBox2D::convertAngle(units::Angle angle) const noexcept
{
    return angle.value();
}

/* ************************************************************************ */

VelocityVector ConverterBox2D::convertLinearVelocity(b2Vec2 velocity) const noexcept
{
    const auto coeff = getTimeCoefficient() / getLengthCoefficient();

    return coeff * VelocityVector{
        units::Velocity(velocity.x),
        units::Velocity(velocity.y)
    };
}

/* ************************************************************************ */

b2Vec2 ConverterBox2D::convertLinearVelocity(VelocityVector velocity) const noexcept
{
    const auto coeff = getLengthCoefficient() / getTimeCoefficient();

    return coeff * b2Vec2(
        static_cast<float32>(velocity.getX().value()),
        static_cast<float32>(velocity.getY().value())
    );
}

/* ************************************************************************ */

units::AngularVelocity ConverterBox2D::convertAngularVelocity(float32 velocity) const noexcept
{
    const auto coeff = getTimeCoefficient();

    return coeff * units::AngularVelocity{velocity};
}

/* ************************************************************************ */

float32 ConverterBox2D::convertAngularVelocity(units::AngularVelocity velocity) const noexcept
{
    const auto coeff = 1.0 / getTimeCoefficient();

    return coeff * velocity.value();
}

/* ************************************************************************ */

AccelerationVector ConverterBox2D::convertLinearAcceleration(b2Vec2 acceleration) const noexcept
{
    const auto coeff = (getTimeCoefficient() * getTimeCoefficient()) / getLengthCoefficient();

    return coeff * AccelerationVector(
        units::Acceleration(acceleration.x),
        units::Acceleration(acceleration.y)
    );
}

/* ************************************************************************ */

b2Vec2 ConverterBox2D::convertLinearAcceleration(AccelerationVector acceleration) const noexcept
{
    const auto coeff = getLengthCoefficient() / (getTimeCoefficient() * getTimeCoefficient());

    return coeff * b2Vec2(
        static_cast<float32>(acceleration.getX().value()),
        static_cast<float32>(acceleration.getY().value())
    );
}

/* ************************************************************************ */

units::Acceleration ConverterBox2D::convertAngularAcceleration(float32 acceleration) const noexcept
{
    const auto coeff = 1;

    return coeff * units::Acceleration(acceleration);
}

/* ************************************************************************ */

float32 ConverterBox2D::convertAngularAcceleration(units::Acceleration acceleration) const noexcept
{
    const auto coeff = 1;

    return coeff * acceleration.value();
}

/* ************************************************************************ */

ForceVector ConverterBox2D::convertForce(b2Vec2 force) const noexcept
{
    const auto coeff = (getTimeCoefficient() * getTimeCoefficient()) / getLengthCoefficient();

    return coeff * ForceVector(
        units::Force(force.x),
        units::Force(force.y)
    );
}

/* ************************************************************************ */

b2Vec2 ConverterBox2D::convertForce(ForceVector force) const noexcept
{
    const auto coeff = getLengthCoefficient() / (getTimeCoefficient() * getTimeCoefficient());

    return coeff * b2Vec2{
        static_cast<float32>(force.getX().value()),
        static_cast<float32>(force.getY().value())
    };
}

/* ************************************************************************ */

units::Force ConverterBox2D::convertTorque(float32 torque) const noexcept
{
    const auto coeff = (getTimeCoefficient() * getTimeCoefficient()) / getLengthCoefficient();

    return coeff * units::Force{torque};
}

/* ************************************************************************ */

float32 ConverterBox2D::convertTorque(units::Force torque) const noexcept
{
    const auto coeff = getLengthCoefficient() / (getTimeCoefficient() * getTimeCoefficient());

    return coeff * torque.value();
}

/* ************************************************************************ */

ImpulseVector ConverterBox2D::convertLinearImpulse(b2Vec2 impulse) const noexcept
{
    const auto coeff = getTimeCoefficient() / getLengthCoefficient();

    return coeff * ImpulseVector(
        units::Impulse(impulse.x),
        units::Impulse(impulse.y)
    );
}

/* ************************************************************************ */

b2Vec2 ConverterBox2D::convertLinearImpulse(ImpulseVector impulse) const noexcept
{
    const auto coeff = getLengthCoefficient() / getTimeCoefficient();

    return coeff * b2Vec2{
        static_cast<float32>(impulse.getX().value()),
        static_cast<float32>(impulse.getY().value())
    };
}

/* ************************************************************************ */

units::Impulse ConverterBox2D::convertAngularImpulse(float32 impulse) const noexcept
{
    const auto coeff = getTimeCoefficient() / getLengthCoefficient();

    return coeff * units::Impulse{impulse};
}

/* ************************************************************************ */

float32 ConverterBox2D::convertAngularImpulse(units::Impulse impulse) const noexcept
{
    const auto coeff = getLengthCoefficient() / getTimeCoefficient();

    return coeff * impulse.value();
}

/* ************************************************************************ */

units::Mass ConverterBox2D::convertMass(float32 mass) const noexcept
{
    return units::Mass(mass);
}

/* ************************************************************************ */

float32 ConverterBox2D::convertMass(units::Mass mass) const noexcept
{
    return mass.value();
}

/* ************************************************************************ */

units::Density ConverterBox2D::convertDensity(float32 density) const noexcept
{
    const auto coeff = 1.0 / (getLengthCoefficient() * getLengthCoefficient() * getLengthCoefficient());

    return coeff * units::Density(density);
}

/* ************************************************************************ */

float32 ConverterBox2D::convertDensity(units::Density density) const noexcept
{
    const auto coeff = getLengthCoefficient() * getLengthCoefficient() * getLengthCoefficient();

    return coeff * density.value();
}

/* ************************************************************************ */

units::Length ConverterBox2D::getMaxObjectTranslation() const noexcept
{
    const auto coeff = 1.0 / getLengthCoefficient();

    return coeff * units::Length{b2_maxTranslation};
}

/* ************************************************************************ */

ConverterBox2D& ConverterBox2D::getInstance() noexcept
{
    static ConverterBox2D instance;
    return instance;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
