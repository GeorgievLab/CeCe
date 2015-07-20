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

#include "core/Units.hpp"
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

using RateType = long double;

template<typename T>
class Reactions
{
protected:

    using PropensityType = RateType;

    DynamicArray<RateType> m_rates;
    DynamicArray<String> m_ids;
    DynamicArray<DynamicArray<T>> m_rules;
    DynamicArray<PropensityType> propensities;

/* ************************************************************************ */

    template<typename Executor, typename Refresher>
    void executeReactions(units::Time step, Executor execute, Refresher refresh)
    {
        if (propensities.empty())
            refresh();

        // initialize random generators
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> rand(0, 1);

        // initialize time
        units::Duration time = units::Duration(0);

        // Gillespie algorithm + tau-leaping
        while (time < step)
        {
            PropensityType sum = std::accumulate(propensities.begin(), propensities.end(), 0.0f);
            if (sum == 0)
            {
                propensities.clear();
                refresh();
                return;
            }

            std::discrete_distribution<> distr(propensities.begin(), propensities.end());

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

    static plugin::cell::CellBase& getCell(simulator::Object& object)
    {
        // initialize cell
        if (!object.is<plugin::cell::CellBase>())
            throw RuntimeException("Only object type cell is allowed to have a reaction.");
        return *object.cast<plugin::cell::CellBase>();
    }

/* ************************************************************************ */

    int getIndexOfMoleculeColumn(const String& id)
    {
        auto pointer = std::find(m_ids.begin(), m_ids.end(), id);
        if (pointer == m_ids.end())
        {
            for (unsigned int i = 0; i < m_rules.size(); i++)
            {
                m_rules[i].emplace_back();
            }
            m_ids.push_back(id);
            return m_ids.size() - 1;
        }
        return std::distance(m_ids.begin(), pointer);
    }

/* ************************************************************************ */

    void extendIntracellular(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate)
    {
        DynamicArray<T> array;
        if (m_rules.size() > 0)
            array.resize(m_rules[0].size());
        for (unsigned int i = 0; i < ids_minus.size(); i++)
        {
            if (ids_minus[i] == "null")
                continue;
            if (ids_minus[i] == "env")
            {
                Log::warning("ENV tag must be alone.");
                return;
            }
            unsigned int index = getIndexOfMoleculeColumn(ids_minus[i]);
            if (index == array.size())
            {
                array.push_back(T{1,0});
                continue;
            }
            array[index].requirement += 1;
        }
        for (unsigned int i = 0; i < ids_plus.size(); i++)
        {
            if (ids_plus[i] == "null")
                continue;
            if (ids_plus[i] == "env")
            {
                Log::warning("ENV tag must be alone.");
                return;
            }
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
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
