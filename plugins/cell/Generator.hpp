
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/String.hpp"
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "simulator/Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace cell {

/* ************************************************************************ */

/**
 * @brief Cell generator module.
 */
class Generator : public simulator::Module
{

// Public Accessors
public:


    /**
     * @brief Returns generated object class.
     *
     * @return
     */
    const String& getObjectClass() const NOEXCEPT
    {
        return m_objectClass;
    }


    /**
     * @brief Returns probability of geneating object.
     *
     * @return
     */
    units::Probability getProbability() const NOEXCEPT
    {
        return m_probability;
    }


    /**
     * @brief Returns start velocity.
     *
     * @return
     */
    const VelocityVector& getStartVelocity() const NOEXCEPT
    {
        return m_startVelocity;
    }


// Public Mutators
public:


    /**
     * @brief Set generated object class.
     *
     * @param objectClass
     */
    void setObjectClass(const String& objectClass) NOEXCEPT
    {
        m_objectClass = objectClass;
    }


    /**
     * @brief Set probability of generating object.
     *
     * @param p
     */
    void setProbability(units::Probability p) NOEXCEPT
    {
        m_probability = p;
    }


    /**
     * @brief Set object start velocity.
     *
     * @param velocity
     */
    void setStartVelocity(VelocityVector velocity)
    {
        m_startVelocity = std::move(velocity);
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt     Simulation time step.
     * @param world  World object.
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


// Private Data Members
private:

    /// Generated object class.
    String m_objectClass;

    /// Probability of generating object.
    units::Probability m_probability = 0.5f;

    /// Initial object velocity.
    VelocityVector m_startVelocity = {units::um_s(10), units::um_s(0)};

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
