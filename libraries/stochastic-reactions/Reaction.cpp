#include "Reaction.hpp"
#include "simulator/Object.hpp"
#include "core/Units.hpp"
#include "core/Log.hpp"

void Reaction::operator()(simulator::Object& object, units::Duration step)
{
    // TODO dependency graph
    // TODO map as reference
    
    auto map = object.getValue<Map<String, int>>("molecule_count");
    DynamicArray<unsigned int> propensities;
    
    // compute propensities
    for (unsigned int i = 0; i < m_rules.size(); i++)
    {
        float local = m_rates[i];
        for (unsigned int j = 0; i < m_rules[i].size(); j++)
        {
            if (m_rules[i][j].requirement == 0)
            {
                continue;
            }
            else
            {
                local *= map[m_ids[j]];
            }
        }
        propensities.push_back(local);
    }
    
    // initialize random generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0, 1);
    std::discrete_distribution<> distr(propensities.begin(), propensities.end());
    
    // tau-leaping
    units::Duration time = 0;
    float sum = std::accumulate(propensities.begin(), propensities.end(), 0.0f);
    while(time < step)
    {
        float delta_time = (1 / sum) * std::log(rand(gen));
        time -= delta_time;
        int index = distr(gen);
        for (unsigned int i = 0; i < m_ids.size(); i++)
        {
            map[m_ids[i]] += m_rules[index][i].product - m_rules[index][i].requirement;
        }
    }
    object.setValue("molecule_count", map);
    
    // TO-REMOVE output log
    for (const auto& value : map)
    {
        Log::info(value.first, ": ", value.second);
    }
}

int Reaction::get_index_of(const String& id)
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
        unsigned int index = get_index_of(ids_minus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{1,0});
            continue;
        }
        array[index].requirement += 1;
    }
    for (unsigned int i = 0; i < ids_plus.size(); i++)
    {
        unsigned int index = get_index_of(ids_plus[i]);
        if (index == array.size())
        {
            array.push_back(ReqProd{0,1});
            continue;
        }
        array[index].product += 1;
    }
    m_rules.push_back(array);
}