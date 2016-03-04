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

// CeCe config
#include "cece/config.hpp"

/* ************************************************************************ */

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/module/Module.hpp"
#include "cece/config/Configuration.hpp"
#include "cece/simulator/IterationType.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace object_generator {

/* ************************************************************************ */

/**
 * @brief Structure for storing object distribution.
 */
struct Distribution
{
    enum class Type
    {
        Uniform,
        Normal
    };

    /// Distribution type.
    Type type = Type::Uniform;

    /// Distribution parameters.
    StaticArray<units::Length, 2> parameters;
};

/* ************************************************************************ */

/**
 * @brief Structure for storing created object parameters.
 */
struct ObjectDesc
{
    // Object spawn rate type.
    using SpawnRate = typename units::Divide<units::Probability, units::Time>::type;

    /// Position distributions.
    using Distributions = StaticArray<Distribution, config::DIMENSION>;


    /// Object class name.
    String className;

    /// Rate of object spawning.
    SpawnRate rate;

    /// Axis distributions.
    Distributions distributions;

    /// List of iteration ranges when the generator is active.
    DynamicArray<Pair<simulator::IterationNumber, simulator::IterationNumber>> active;

    /// Object configuration
    config::Configuration config;
};

/* ************************************************************************ */

/**
 * @brief Object generator module.
 */
class Module : public module::Module
{

// Public Ctors & Dtors
public:


    using module::Module::Module;


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
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Store module configuration.
     *
     * @param config Output configuration.
     */
    void storeConfig(config::Configuration& config) const override;


    /**
     * @brief Update module state.
     */
    void update() override;


// Private Data Members
private:


    /// List of generated objects.
    DynamicArray<ObjectDesc> m_objects;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
