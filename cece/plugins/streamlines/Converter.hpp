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

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/Vector.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/UnitsCtors.hpp"
#include "cece/core/VectorUnits.hpp"

/* ************************************************************************ */

namespace cece { namespace config { class Configuration; } }

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief LB units converter.
 */
class Converter
{


// Public Accessors
public:


    /**
     * @brief Returns fluid kinematic viscosity.
     *
     * @return
     */
    units::KinematicViscosity getKinematicViscosity() const noexcept
    {
        return m_kinematicViscosity;
    }


    /**
     * @brief Returns characteristic length.
     *
     * @return
     */
    units::Length getCharLength() const noexcept
    {
        return m_charLength;
    }


    /**
     * @brief Returns characteristic time.
     *
     * @return
     */
    units::Time getCharTime() const noexcept
    {
        return m_charTime;
    }


    /**
     * @brief Returns characteristic velocity.
     *
     * @return
     */
    units::Velocity getCharVelocity() const noexcept
    {
        return getCharLength() / getCharTime();
    }


    /**
     * @brief Returns number of nodes in LB along characteristic length.
     *
     * @return
     */
    unsigned int getNumberNodes() const noexcept
    {
        return m_numberNodes;
    }


    /**
     * @brief Returns number of time steps in LB for units conversion.
     *
     * @return
     */
    unsigned int getNumberSteps() const noexcept
    {
        return m_numberSteps;
    }


    /**
     * @brief Returns channel height.
     *
     * @return Channel height.
     */
    units::Length getHeight() const noexcept
    {
        return m_height;
    }


// Public Mutators
public:


    /**
     * @brief Set fluid viscosity.
     *
     * @param viscosity
     */
    void setKinematicViscosity(units::KinematicViscosity viscosity) noexcept
    {
        m_kinematicViscosity = viscosity;
    }


    /**
     * @brief Set characteristic length.
     *
     * @param length
     */
    void setCharLength(units::Length length) noexcept
    {
        m_charLength = length;
    }


    /**
     * @brief Set characteristic time.
     *
     * @param time
     */
    void setCharTime(units::Time time) noexcept
    {
        m_charTime = time;
    }


    /**
     * @brief Set number of nodes in LB for units conversion.
     *
     * @param nodes
     */
    void setNumberNodes(unsigned int nodes) noexcept
    {
        Assert(nodes > 0);
        m_numberNodes = nodes;
    }


    /**
     * @brief Set number of time steps in LB for units conversion.
     *
     * @param steps
     */
    void setNumberSteps(unsigned int steps) noexcept
    {
        Assert(steps > 0);
        m_numberSteps = steps;
    }


    /**
     * @brief Set channel height.
     *
     * @param height New height.
     */
    void setHeight(units::Length height) noexcept
    {
        m_height = height;
    }


// Public Operations
public:


    /**
     * @brief Convert length from LB to physical.
     *
     * @param length
     *
     * @return
     */
    units::Length convertLength(RealType length) const noexcept
    {
        const auto charLength = m_charLength / getNumberNodes();
        return charLength * length;
    }


    /**
     * @brief Convert length from physical to LB.
     *
     * @param length
     *
     * @return
     */
    RealType convertLength(units::Length length) const noexcept
    {
        const auto charLength = m_charLength / getNumberNodes();
        return length / charLength;
    }


    /**
     * @brief Convert velocity from LB to physical.
     *
     * @param vel
     *
     * @return
     */
    units::Velocity convertVelocity(RealType vel) const noexcept
    {
        const auto charTime = m_charTime / getNumberSteps();
        const auto charLength = m_charLength / getNumberNodes();
        return charLength / charTime * vel;
    }


    /**
     * @brief Convert velocity from LB to physical.
     *
     * @param vel
     *
     * @return
     */
    units::VelocityVector convertVelocity(Vector<RealType> vel) const noexcept
    {
        const auto charTime = m_charTime / getNumberSteps();
        const auto charLength = m_charLength / getNumberNodes();
        return charLength / charTime * vel;
    }


    /**
     * @brief Convert velocity from physical to LB.
     *
     * @param vel
     *
     * @return
     */
    RealType convertVelocity(units::Velocity vel) const noexcept
    {
        const auto charTime = m_charTime / getNumberSteps();
        const auto charLength = m_charLength / getNumberNodes();
        return charTime / charLength * vel;
    }


    /**
     * @brief Convert velocity from physical to LB.
     *
     * @param vel
     *
     * @return
     */
    Vector<RealType> convertVelocity(units::VelocityVector vel) const noexcept
    {
        const auto charTime = m_charTime / getNumberSteps();
        const auto charLength = m_charLength / getNumberNodes();
        return charTime / charLength * vel;
    }


    /**
     * @brief Calculate viscosity from relaxation time.
     *
     * @return
     */
    RealType calculateViscosity() const noexcept;


    /**
     * @brief Calculate viscosity from relaxation time.
     *
     * @return
     */
    RealType calculateTau() const noexcept;


    /**
     * @brief Calculate relaxation frequency.
     *
     * @return
     */
    RealType calculateOmega() const noexcept
    {
        return 1.0 / calculateTau();
    }


    /**
     * @brief Calculate number of time steps from tau.
     *
     * @param tau
     *
     * @return
     */
    unsigned int calculateNumberSteps(RealType tau) const noexcept;


    /**
     * @brief Calculate Reynolds number.
     *
     * @return
     */
    RealType calculateRe() const noexcept
    {
        return getCharLength() * getCharLength() / getCharTime() / getKinematicViscosity();
    }


    /**
     * @brief Calculate mean velocity from volumeric flow rate.
     *
     * @param flow  Volumeric flow rate.
     * @param width Inlet width.
     *
     * @return Mean velocity.
     *
     * @note Height is taken from internal value.
     */
    units::Velocity calculateVelocity(units::VolumericFlow flow, units::Length width) const noexcept
    {
        return calculateVelocity(flow, width * getHeight());
    }


    /**
     * @brief Calculate mean velocity from volumeric flow rate.
     *
     * @param flow Volumeric flow rate.
     * @param area Flow area.
     *
     * @return Mean velocity.
     *
     * @note Height is taken from internal value.
     */
    units::Velocity calculateVelocity(units::VolumericFlow flow, units::Area area) const noexcept
    {
        return flow / area;
    }


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config);


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const;


// Private Data Members
private:

    /// Fluid viscosity (of Water).
    units::KinematicViscosity m_kinematicViscosity = units::mm2_s(0.658);

    /// Characteristic length.
    units::Length m_charLength = units::um(1);

    /// Characteristic time.
    units::Time m_charTime = units::s(1);

    /// Number of LB nodes for units conversions.
    unsigned int m_numberNodes = 1;

    /// Number of LB time steps for units conversions
    unsigned int m_numberSteps = 1;

    /// Streamlines channel height.
    units::Length m_height = units::um(1);
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
