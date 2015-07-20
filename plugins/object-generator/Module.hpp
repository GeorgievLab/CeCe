/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/String.hpp"
#include "core/Units.hpp"
#include "core/VectorUnits.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace object_generator {

/* ************************************************************************ */

/**
 * @brief Structure for storing created object parameters.
 */
struct ObjectDesc
{
    /// Object class name.
    String className;

    /// Probability of object creation.
    units::Probability probability;

    /// Minimum spawn position.
    PositionVector positionMin;

    /// Maximum spawn position.
    PositionVector positionMax;

    /// Object programs.
    DynamicArray<String> programs;
};

/* ************************************************************************ */

/**
 * @brief Object generator module.
 */
class Module : public simulator::Module
{

// Public Accessors
public:





// Public Mutators
public:


    /**
     * @brief Register object.
     *
     * @param desc Object description.
     */
    void add(ObjectDesc desc)
    {
        m_objects.push_back(std::move(desc));
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


    /// List of generated objects.
    DynamicArray<ObjectDesc> m_objects;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
