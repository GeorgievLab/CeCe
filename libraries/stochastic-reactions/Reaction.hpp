#pragma once

#include "simulator/Object.hpp"
#include "core/Units.hpp"
#include "core/DynamicArray.hpp"
#include "core/Grid.hpp"

class Reaction
{
    DynamicArray<float> m_rates;
    DynamicArray<String> m_ids;
    DynamicArray<DynamicArray<int>> m_rules;
    
    public:
    void operator()(simulator::Object& object, units::Duration);
};