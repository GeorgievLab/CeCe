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
#include <random>
#include <algorithm>

#include "core/String.hpp"
#include "core/Assert.hpp"
#include "core/StringView.hpp"
#include "core/Units.hpp"
#include "core/DynamicArray.hpp"
#include "core/Log.hpp"
#include "core/Exception.hpp"

#include "simulator/Object.hpp"
#include "simulator/Simulation.hpp"
#include "plugins/cell/CellBase.hpp"
#include "Diffusion.hpp"

#include "Types.hpp"
#include "Context.hpp"
#include "Reaction.hpp"
#include "Function.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

/**
 * @brief Base class for reactions implementations.
 */
class Reactions
{

// Public Accessors:
public:

    /**
     * @brief Returns number of different molecules in reactions.
     *
     * @return
     */
    inline unsigned int getMoleculeCount() const noexcept
    {
        return m_moleculeNames.size();
    }

    /**
     * @brief Check if given molecule is used in reactions.
     *
     * @param name Molecule name.
     *
     * @return
     */
    inline bool hasMolecule(const String& name) const noexcept
    {
        return std::find(m_moleculeNames.begin(), m_moleculeNames.end(), name) != m_moleculeNames.end();
    }

    /**
     * @brief Returns molecule name.
     *
     * @param id Molecule identifier.
     *
     * @return
     */
    inline const String& getMoleculeName(unsigned int index) const noexcept
    {
        return m_moleculeNames[index];
    }


    /**
     * @brief Returns number of reactions.
     *
     * @return
     */
    inline unsigned int getReactionCount() const noexcept
    {
        return m_reactions.size();
    }


    /**
     * @brief Evaulates rate.
     *
     * @param reaction Reaction identifier.
     *
     * @return rate
     */
    inline RateType evalRate(unsigned int index, const Context& pointers) const noexcept
    {
        return m_reactions[index].evaluateRate(pointers);
    }


    /**
     * @brief Evaulates condition.
     *
     * @param reaction Reaction identifier.
     *
     * @return condition was satisfied or not
     */
    inline bool evalCond(unsigned int index, const Context& pointers) const noexcept
    {
        return m_reactions[index].evaluateCondition(pointers);
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @param reaction Reaction identifier.
     *
     * @return
     */
    inline Reaction& getReaction(unsigned int index) noexcept
    {
        return m_reactions[index];
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @param reaction Reaction identifier.
     *
     * @return
     */
    inline const Reaction& getReaction(unsigned int index) const noexcept
    {
        return m_reactions[index];
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @return
     */
    inline Reaction& getLastReaction() noexcept
    {
        Assert(!m_reactions.empty());
        return m_reactions.back();
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @return
     */
    inline const Reaction& getLastReaction() const noexcept
    {
        Assert(!m_reactions.empty());
        return m_reactions.back();
    }


    /**
     * @brief Searches global bool functions for the one with given ID
     *
     * @return
     */
    inline SharedPtr<Node<bool>> getGlobalBoolFunction(const String& id) const
    {
        auto search = m_boolFunctions.find(id);
        if(search != m_boolFunctions.end())
            return search->second;
        return nullptr;
    }


    /**
     * @brief Searches global real functions for the one with given ID
     *
     * @return
     */
    inline SharedPtr<Node<RealType>> getGlobalRealFunction(const String& id) const
    {
        auto search = m_realFunctions.find(id);
        if(search != m_realFunctions.end())
            return search->second;
        return nullptr;
    }

// Private Operations
private:

    /**
     * @brief Execute reactions each step.
     *
     * @tparam Executor  Function called when reaction is executed.
     * @tparam Refresher Function called when reaction propensities are recomputed.
     *
     * @param step
     */
    void executeReactions(units::Time step, const Context& pointers);

    /**
     * @brief Computes propensity of given reaction.
     *
     * @param index of row, cell, diffusion
     * @return propensity
     */
    void executeRules(unsigned int index, const Context& pointers);

     /**
     * @brief Computes propensity of given reaction.
     *
     * @param index of row, cell, diffusion
     * @return propensity
     */
    PropensityType computePropensity(const unsigned int index, const Context& pointers);

    /**
     * @brief Computes propensities of all reactions.
     *
     * @param cell, diffusion
     * @return
     */
    void initializePropensities(const Context& pointers);

    /**
     * @brief Refreshes propensities of ractions which have requirements of specific molecule.
     *
     * @param index of column, cell, diffusion
     */
    void refreshPropensities(const unsigned int index, const Context& pointers);

    /**
     * @brief Computes propensities of all reactions that depends on environment.
     *
     * @param cell, diffusion
     * @return
     */
    void refreshEnvPropensities(const Context& pointers);


    /**
     * @brief Function that releases or absorbs the molecules outside the cell.
     *
     * @param id of molecule, number of molecules, diffusion
     */
    void changeMoleculesInEnvironment(const int change, const String& id, const Context& pointers);

public:

    /**
     * @brief Plugin functor.
     */
    void operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration step);

    /**
     * @brief Returns molecule inner identifier. In case the molecule doesn't exists
     * in reactions it's registered.
     *
     * @param Name of molucule.
     *
     * @return Molecule index.
     */
    unsigned int getMoleculeIndex(const String& name);

    /**
     * @brief Extend matrix using rules for intracellular reactions.
     *
     * @param reaction
     */
    inline void extend(const Reaction& reaction)
    {
        m_reactions.push_back(reaction);
    }

// Private Data Members
private:

    /// List of molecule names.
    DynamicArray<String> m_moleculeNames;

    /// List of reactions.
    DynamicArray<Reaction> m_reactions;

    /// Computed reactions propensities.
    DynamicArray<PropensityType> m_propensities;

    /// Map of global defined functions.
    Map<String, SharedPtr<Node<bool>>> m_boolFunctions;
    Map<String, SharedPtr<Node<RealType>>> m_realFunctions;
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
