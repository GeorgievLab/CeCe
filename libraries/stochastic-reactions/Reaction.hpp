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
    
    void addExpression(String id, float rate);
    
    void addDegradation(String id, float rate);
    
    void addAssemble(String id1, String id2, String complex, float rate);
    
    void addDisassemble(String complex, String id1, String id2, float rate);
    
    private:
    int get_index_of(String id);
};