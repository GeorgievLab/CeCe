#pragma once

#include "simulator/Object.hpp"
#include "core/Units.hpp"
#include "core/DynamicArray.hpp"
#include "core/Grid.hpp"

class Reaction
{
    public:
    DynamicArray<float> m_rates;
    DynamicArray<String> m_ids;
    DynamicArray<DynamicArray<int>> m_rules;
    
    void operator()(simulator::Object& object, units::Duration);
    
    int get_index_of(const String& id);
    
    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, float& rate);
};