
// Declaration
#include "Reaction.hpp"

// C++
#include <random>

// Simulator
#include "core/Units.hpp"
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "simulator/Object.hpp"

// Plugins
#include "plugins/cell/CellBase.hpp"

void Reaction::operator()(simulator::Object& object, simulator::Simulation&, units::Duration step)
{
    // initialize cell
    if (!object.is<plugin::cell::CellBase>())
        throw RuntimeException("Only object type cell is allowed to have a reaction.");
    cell = object.cast<plugin::cell::CellBase>();

    // initialize random generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0, 1);
    std::discrete_distribution<> distr(propensities.begin(), propensities.end());

    // tau-leaping
    units::Duration time = units::Duration(0);
    if (propensities.size() == 0)
    {
        initializePropensities();
    }
    float sum = std::accumulate(propensities.begin(), propensities.end(), 0.0f);
    if (sum == 0)
        return;
    while(time < step)
    {
        auto delta_time = units::Duration((1 / sum) * std::log(rand(gen)));
        time -= delta_time;
        int index = distr(gen);
        for (unsigned int i = 0; i < m_ids.size(); i++)
        {
            int change = m_rules[index][i].product - m_rules[index][i].requirement;
            if (change != 0)
            {
                cell->addMolecules(m_ids[i], change);
                refreshPropensities(i);
            }
        }
    }
}

void Reaction::initializePropensities()
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        propensities.push_back(computePropensity(i));
    }
}

void Reaction::refreshPropensities(unsigned int index)
{
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        if (m_rules[i][index].requirement != 0)
        {
            propensities[i] = computePropensity(i);
        }
    }
}

float Reaction::computePropensity(unsigned int index)
{
    float local = m_rates[index];
    for (unsigned int i = 0; i < m_rules[i].size(); i++)
    {
        if (m_rules[index][i].requirement == 0)
        {
            continue;
        }
        else
        {
            local *= cell->getMoleculeCount(m_ids[i]);
        }
    }
    return local;
}

int Reaction::getIndexOf(const String& id)
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

void Reaction::extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, float rate)
{
    m_rates.push_back(rate);
    DynamicArray<ReqProd> array;
    if (m_rules.size() > 0)
        array.resize(m_rules[0].size());
    for (unsigned int i = 0; i < ids_minus.size(); i++)
    {
        unsigned int index = getIndexOf(ids_minus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{1,0});
            continue;
        }
        array[index].requirement += 1;
    }
    for (unsigned int i = 0; i < ids_plus.size(); i++)
    {
        unsigned int index = getIndexOf(ids_plus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{0,1});
            continue;
        }
        array[index].product += 1;
    }
    m_rules.push_back(array);
}

bool Reaction::operator ==(const Reaction& rhs)
{
    if (rhs.m_rates.size() != m_rates.size() && rhs.m_ids.size() != m_ids.size())
        return false;
    DynamicArray<unsigned int> skip;
    bool valid;
    for (unsigned int i = 0; i < m_rates.size(); i++)
    {
        valid = false;
        for (unsigned int j = 0; j < rhs.m_rates.size(); i++)
        {
            if (std::find(skip.begin(), skip.end(), j) != skip.end())
                continue;
            if (areEqualReactions(rhs, i, j))
            {
                valid = true;
                skip.push_back(j);
                break;
            }
            continue;
        }
        if (!valid)
            return false;
    }
    return true;
}

bool Reaction::areEqualReactions(const Reaction& rhs, unsigned int index1, unsigned int index2)
{
    if (m_rates[index1] != rhs.m_rates[index2])
        return false;
    for (unsigned int i = 0; i < m_ids.size(); i++)
    {
        auto pointer = std::find(rhs.m_ids.begin(), rhs.m_ids.end(), m_ids[i]);
        if (pointer == rhs.m_ids.end())
            return false;
        unsigned int index = std::distance(rhs.m_ids.begin(), pointer);
        if (rhs.m_ids[index] != m_ids[i] || rhs.m_rules[index2][index] != m_rules[index1][i])
            return false;
    }
    return true;
}
