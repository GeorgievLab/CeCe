#include "Reaction.hpp"
#include "simulator/Object.hpp"
#include "core/Units.hpp"

void Reaction::operator()(simulator::Object& object, units::Duration step)
{
    // TODO dependency graph
    // TODO map as reference
    auto map = object.getValue<Map<String, int>>("molecule_count");
    float sum = std::accumulate(m_rates.begin(), m_rates.end(), 0.0f);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand(0, 1);
    std::discrete_distribution<> distr(m_rates.begin(), m_rates.end());
    units::Duration time = 0;
    while(time < step)
    {
        float delta_time = (1 / sum) * std::log(rand(gen));
        time += delta_time;
        int index = distr(gen);
        for (unsigned int i = 0; i < m_ids.size(); i++)
        {
            map[m_ids[i]] += m_rules[index][i];
        }
    }
    object.setValue("molecule_count", map);
}

void Reaction::addExpression(String id, float rate)
{
    DynamicArray<int> local (m_ids.size());
    local[get_index_of(id)] = 1;
    m_rates.push_back(rate);
    m_rules.push_back(local);
}

void Reaction::addDegradation(String id, float rate)
{
    DynamicArray<int> local (m_ids.size());
    int index = get_index_of(id);
    local[index] = -1;
    m_rates.push_back(rate);
    m_rules.push_back(local);
}

void Reaction::addAssemble(String id1, String id2, String complex, float rate)
{
    DynamicArray<int> local (m_ids.size());
    local[get_index_of(id1)] = -1;
    local[get_index_of(id2)] = -1;
    local[get_index_of(complex)] = 1;
    m_rates.push_back(rate);
    m_rules.push_back(local);
}

void Reaction::addDisassemble(String complex, String id1, String id2, float rate)
{
    DynamicArray<int> local (m_ids.size());
    local[get_index_of(complex)] = -1;
    local[get_index_of(id1)] = 1;
    local[get_index_of(id2)] = 1;
    m_rates.push_back(rate);
    m_rules.push_back(local);
}

int Reaction::get_index_of(String id)
{
    auto pointer = std::find(m_ids.begin(), m_ids.end(), id);
    if (pointer == m_ids.end())
    {
        for(auto& rule : m_rules)
        {
            rule.push_back(0);
        }
        m_ids.push_back(id);
        return m_ids.size() - 1;
    }
    return std::distance(m_ids.begin(), pointer);
}