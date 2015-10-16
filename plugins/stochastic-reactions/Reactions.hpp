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
#include "core/StringView.hpp"
#include "core/Units.hpp"
#include "core/Real.hpp"
#include "core/DynamicArray.hpp"
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "simulator/Object.hpp"
#include "simulator/Simulation.hpp"

#include "../cell/CellBase.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

/**
 * @brief Base class for reactions implementations.
 *
 * @tparam T Inner data structure type.
 */
class Reactions
{

// Public Types
public:

    /// Type used for reaction rates.
    using RateType = RealType;

// Public Structures
public:

    /**
     * @brief Reaction condition description structure.
     */
    struct Conditions
    {

    };

    /**
     * @brief Member of reactions stochiometric matrix.
     */
    struct ReqProd
    {
        unsigned int requirement;
        unsigned int product;
        unsigned int env_requirement;
        unsigned int env_product;

        ReqProd(unsigned int req = 0, unsigned int prod = 0, unsigned int ereq = 0, unsigned int eprod = 0) noexcept
        : requirement(req), product(prod), env_requirement(ereq), env_product(eprod)
        {
            // Nothing to do
        }
    };

    /**
     * @brief Reaction data, rules and condition.
     */
    struct Reaction
    {
        RateType rate;
        DynamicArray<ReqProd> rules;
        Conditions conditions;
    };

// Public Accessors:
public:

    /**
     * @brief Returns number of different molecules in reactions.
     *
     * @return
     */
    unsigned int getMoleculeCount() const noexcept
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
    bool hasMolecule(const String& name) const noexcept
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
    const String& getMoleculeName(unsigned int index) const noexcept
    {
        return m_moleculeNames[index];
    }


    /**
     * @brief Returns number of reactions.
     *
     * @return
     */
    unsigned int getReactionCount() const noexcept
    {
        return m_reactions.size();
    }


    /**
     * @brief Returns reaction rate.
     *
     * @param reaction Reaction identifier.
     *
     * @return
     */
    RateType getRate(unsigned int index) const noexcept
    {
        return m_reactions[index].rate;
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @param reaction Reaction identifier.
     *
     * @return
     */
    Reaction& getReaction(unsigned int index) noexcept
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
    const Reaction& getReaction(unsigned int index) const noexcept
    {
        return m_reactions[index];
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @return
     */
    Reaction& getLastReaction() noexcept
    {
        assert(!m_reactions.empty());
        return m_reactions.back();
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @return
     */
    const Reaction& getLastReaction() const noexcept
    {
        assert(!m_reactions.empty());
        return m_reactions.back();
    }


// Public Mutators
public:

    /**
     * @brief Add execute condition to last reaction.
     *
     * @param condition Reaction condition.
     * @param noCond    Reaction without condition modifications.
     */
    void addCondition()
    {

    }


// Protected Types
protected:

    /// Propensity type.
    using PropensityType = RateType;


// Protected Operations
protected:


    /**
     * @brief Execute reactions each step.
     *
     * @tparam Executor  Function called when reaction is executed.
     * @tparam Refresher Function called when reaction propensities are recomputed.
     *
     * @param step
     */
    void executeReactions(units::Time step)
    {
        if (m_propensities.empty())
            // refresh
            ;

        // initialize time
        units::Time time = Zero;

        // initialize random
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> rand(0, 1);

        // Gillespie algorithm + tau-leaping + dependency graph(hidden inside execute reaction)
        while (time < step)
        {
            PropensityType sum = std::accumulate(m_propensities.begin(), m_propensities.end(), 0.0f);

            if (sum == 0)
            {
                //refresh
                return;
            }

            std::discrete_distribution<> distr(m_propensities.begin(), m_propensities.end());

            // time of reaction
            const auto delta_time = units::Duration((1 / sum) * std::log(rand(gen)));
            time -= delta_time;

            // execute reaction
        }
    }


    /**
     * @brief Returns molecule inner identifier. In case the molecule doesn't exists
     * in reactions it's registered.
     *
     * @param Name of molucule.
     *
     * @return Molecule identifier.
     */
    unsigned int getMoleculeIndex(const String& name)
    {
        auto pointer = std::find(m_moleculeNames.begin(), m_moleculeNames.end(), name);

        // Molecule exists in reactions
        if (pointer != m_moleculeNames.end())
            return std::distance(m_moleculeNames.begin(), pointer);

        // Store molecule
        m_moleculeNames.push_back(name);

        // Resize all reactions
        for (auto& reaction : m_reactions)
            reaction.rules.resize(m_moleculeNames.size());

        return m_moleculeNames.size() - 1;
    }


    /**
     * @brief Extend matrix using rules for intracellular reactions.
     *
     * @param ids_plus
     * @param ids_minus
     * @param rate
     *
    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate)
    {
        // initialize array
        DynamicArray<Reaction> array;
        if (m_reactions.size() > 0)
            array.resize(m_rules[0].size());

        // extend with requirements
        for (unsigned int i = 0; i < ids_minus.size(); i++)
        {
            if (ids_minus[i] == "null")
                continue;
            unsigned int index = getMoleculeIndex(ids_minus[i]);
            if (index == array.size())
            {
                array.push_back(ReqProd(1,0));
                continue;
            }
            array[index].requirement += 1;
        }

        //extend with products
        for (unsigned int i = 0; i < ids_plus.size(); i++)
        {
            if (ids_plus[i] == "null")
                continue;
            unsigned int index = getMoleculeId(ids_plus[i]);
            if (index == array.size())
            {
                array.push_back(T{0,1});
                continue;
            }
            array[index].product += 1;
        }
        m_rates.push_back(rate);
        m_rules.push_back(array);
    }
*/

// Protected Data Members
protected:

    /// List of molecule names.
    DynamicArray<String> m_moleculeNames;

    /// List of reactions.
    DynamicArray<Reaction> m_reactions;

    /// Computed reactions propensities.
    DynamicArray<PropensityType> m_propensities;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
