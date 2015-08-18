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

using RateType = RealType;

template<typename T>
class Reactions
{

// Public Types
public:


    /// Reaction identifier type.
    using ReactionId = unsigned int;

    /// Molecule identifier type.
    using MoleculeId = unsigned int;

protected:

    using PropensityType = RateType;

    DynamicArray<RateType> m_rates;
    DynamicArray<String> m_ids;
    DynamicArray<DynamicArray<T>> m_rules;
    DynamicArray<PropensityType> m_propensities;

/* ************************************************************************ */
    /**
     * @brief Execute reactions each step.
     *
     * @return
     */
    template<typename Executor, typename Refresher>
    void executeReactions(units::Time step, Executor execute, Refresher refresh)
    {
        if (m_propensities.empty())
            refresh();

        // initialize time
        units::Duration time = units::Duration(0);

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
            auto delta_time = units::Duration((1 / sum) * std::log(rand(gen)));
            time -= delta_time;
            // which reaction happened
            int index = distr(gen);
            // execute reaction
            execute(index);
        }
    }

/* ************************************************************************ */
    /**
     * @brief Casts object to cell type.
     *
     * @param Object.
     *
     * @return Cell.
     */
    static plugin::cell::CellBase& getCell(simulator::Object& object)
    {
        // cast object to cell
        if (!object.is<plugin::cell::CellBase>())
            throw RuntimeException("Only object type cell is allowed to have a reaction.");
        return *object.cast<plugin::cell::CellBase>();
    }

/* ************************************************************************ */
    /**
     * @brief Returns index of molecule's column in reactions matrix.
     *
     * @param Name of molucule.
     *
     * @return Index of column.
     */
    int getIndexOfMoleculeColumn(const String& id)
    {
        auto pointer = std::find(m_ids.begin(), m_ids.end(), id);

        // add given molucule when not present
        if (pointer == m_ids.end())
        {
            for (unsigned int i = 0; i < m_rules.size(); i++)
            {
                m_rules[i].emplace_back();
            }
            m_ids.push_back(id);
            return m_ids.size() - 1;
        }

        // return distance between pointers
        return std::distance(m_ids.begin(), pointer);
    }

/* ************************************************************************ */
    /**
     * @brief Extend matrix using rules for intracellular rections.
     *
     * @param arrays of molucule's IDs, reaction rate
     *
     * @return
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
            unsigned int index = getIndexOfMoleculeColumn(ids_minus[i]);
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
            unsigned int index = getIndexOfMoleculeColumn(ids_plus[i]);
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

/* ************************************************************************ */

public:

/* ************************************************************************ */
    /**
     * @brief Edit reactions rule matrix to suit given reaction conditions.
     *
     * @param molecule name, requirement for execution, reversible flag and index
     *
     * @return
     */
    void addCondition(const String& name, unsigned int requirement, bool clone, DynamicArray<T>& no_cond)
    {
        // add unmodified reaction rule after OR is reached
        if (clone)
        {
            // backup of unmodified reaction can be outdated (shorter)
            no_cond.resize(m_ids.size());

            m_rules.push_back(no_cond);
            m_rates.push_back(m_rates[m_rates.size() - 1]);
        }

        // get index of required molecule
        unsigned int column_index = getIndexOfMoleculeColumn(name);

        // add requirement
        if (requirement == 0)
            m_rules[m_rules.size() - 1][column_index].mustnt_have = true;
        else
        {
            m_rules[m_rules.size() - 1][column_index].requirement = std::max(m_rules[m_rules.size() - 1][column_index].requirement, 1u);
            m_rules[m_rules.size() - 1][column_index].product = std::max(m_rules[m_rules.size() - 1][column_index].product, 1u);
        }
    }

/* ************************************************************************ */

// Public Accessors:
public:

/* ************************************************************************ */
    /**
     * @brief Returns number of different molecules in reactions.
     *
     * @return
     */
    unsigned int getMoleculeCount() const noexcept
    {
        return m_ids.size();
    }

/* ************************************************************************ */
    /**
     * @brief Check if given molecule is used in reactions.
     *
     * @param name Molecule name.
     *
     * @return
     */
    bool hasMolecule(const StringView& name) const noexcept
    {
        return std::find(m_ids.begin(), m_ids.end(), name) != m_ids.end();
    }

/* ************************************************************************ */
    /**
     * @brief Returns molecule name.
     *
     * @param id Molecule identifier.
     *
     * @return
     */
    StringView getMoleculeName(MoleculeId id) const noexcept
    {
        return m_ids[id];
    }

/* ************************************************************************ */
    /**
     * @brief Returns number of reactions.
     *
     * @return
     */
    unsigned int getReactionCount() const noexcept
    {
        return m_rules.size();
    }

/* ************************************************************************ */
    /**
     * @brief Returns number of rates.
     *
     * @return
     */
    unsigned int getRateCount() const noexcept
    {
        return m_rates.size();
    }

/* ************************************************************************ */
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

/* ************************************************************************ */
    /**
     * @brief Returns the last row in reaction rule matrix.
     *
     * @return
     */
    const DynamicArray<T>& getLastReaction() const noexcept
    {
        return m_rules[m_rules.size() - 1];
    }
};
/* ************************************************************************ */

}
}

/* ************************************************************************ */
