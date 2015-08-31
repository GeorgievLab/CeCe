/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
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
template<typename T>
class Reactions
{

// Public Types
public:


    /// Type used for reaction rates.
    using RateType = RealType;

    /// Type for store reaction.
    using ReactionType = DynamicArray<T>;

    /// Reaction identifier type.
    using ReactionId = unsigned int;

    /// Molecule identifier type.
    using MoleculeId = unsigned int;


// Public Structures
public:


    /**
     * @brief Reaction condition description structure.
     */
    struct Condition
    {
        /// Molecule name.
        String name;

        /// Number of required molecules/concentration.
        RealType requirement;

        /// If reaction should be cloned.
        bool clone;

        /// If less operator is used.
        bool less;
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
    const String& getMoleculeName(MoleculeId id) const noexcept
    {
        return m_moleculeNames[id];
    }


    /**
     * @brief Returns number of reactions.
     *
     * @return
     */
    unsigned int getReactionCount() const noexcept
    {
        return m_rules.size();
    }


    /**
     * @brief Returns reaction rate.
     *
     * @param reaction Reaction identifier.
     *
     * @return
     */
    RateType getRate(ReactionId reaction) const noexcept
    {
        return m_rates[reaction];
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @param reaction Reaction identifier.
     *
     * @return
     */
    ReactionType& getReaction(ReactionId reaction) noexcept
    {
        return m_rules[reaction];
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @param reaction Reaction identifier.
     *
     * @return
     */
    const ReactionType& getReaction(ReactionId reaction) const noexcept
    {
        return m_rules[reaction];
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @return
     */
    ReactionType& getLastReaction() noexcept
    {
        assert(!m_rules.empty());
        return m_rules.back();
    }


    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @return
     */
    const ReactionType& getLastReaction() const noexcept
    {
        assert(!m_rules.empty());
        return m_rules.back();
    }


// Public Mutators
public:


    /**
     * @brief Add execute condition to last reaction.
     *
     * @param condition Reaction condition.
     * @param noCond    Reaction without condition modifications.
     */
    void addCondition(const Condition& condition, ReactionType& noCond)
    {
        // add unmodified reaction rule after OR is reached
        if (condition.clone)
        {
            // backup of unmodified reaction can be outdated (shorter)
            noCond.resize(m_moleculeNames.size());

            m_rules.push_back(noCond);
            m_rates.push_back(m_rates[m_rates.size() - 1]);
        }

        // get index of required molecule
        const auto moleculeId = getMoleculeId(condition.name);

        // Alias to last reaction
        auto& reaction = m_rules[m_rules.size() - 1];

        const auto diff = std::max<RealType>(condition.requirement - reaction[moleculeId].requirement, 0);

        // add requirement
        reaction[moleculeId].less = condition.less;
        reaction[moleculeId].requirement += diff;
        reaction[moleculeId].product += diff;
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
     * @param execute
     * @param refresh
     */
    template<typename Executor, typename Refresher>
    void executeReactions(units::Time step, Executor execute, Refresher refresh)
    {
        if (m_propensities.empty())
            refresh();

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
                refresh();
                return;
            }

            std::discrete_distribution<> distr(m_propensities.begin(), m_propensities.end());

            // time of reaction
            const auto delta_time = units::Duration((1 / sum) * std::log(rand(gen)));
            time -= delta_time;

            // execute reaction
            execute(distr(gen));
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
    MoleculeId getMoleculeId(const String& name)
    {
        auto pointer = std::find(m_moleculeNames.begin(), m_moleculeNames.end(), name);

        // Molecule exists in reactions
        if (pointer != m_moleculeNames.end())
            return std::distance(m_moleculeNames.begin(), pointer);

        // Store molecule
        m_moleculeNames.push_back(name);

        // Resize all reactions
        for (auto& reaction : m_rules)
            reaction.resize(m_moleculeNames.size());

        return m_moleculeNames.size() - 1;
    }


    /**
     * @brief Extend matrix using rules for intracellular reactions.
     *
     * @param ids_plus
     * @param ids_minus
     * @param rate
     */
    void extendIntracellular(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate)
    {
        // initialize array
        DynamicArray<T> array;
        if (m_rules.size() > 0)
            array.resize(m_rules[0].size());

        // extend with requirements
        for (unsigned int i = 0; i < ids_minus.size(); i++)
        {
            if (ids_minus[i] == "null")
                continue;
            unsigned int index = getMoleculeId(ids_minus[i]);
            if (index == array.size())
            {
                array.push_back(T{1,0});
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


// Protected Data Members
protected:


    /// List of molecule names.
    DynamicArray<String> m_moleculeNames;

    /// List of reactions rates.
    DynamicArray<RateType> m_rates;

    /// List of reactions.
    DynamicArray<ReactionType> m_rules;

    /// Computed reactions propensities.
    DynamicArray<PropensityType> m_propensities;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
