#pragma once

#include "simulator/Object.hpp"
#include "core/Units.hpp"
#include "core/DynamicArray.hpp"
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "../cell/CellBase.hpp"

class Reaction
{
private: 

    struct ReqProd
    {
        ReqProd() = default;
        
        ReqProd(int req, int prod) NOEXCEPT
        : requirement(req), product(prod)
        {
            // Nothing to do
        }
        
        int requirement = 0;
        int product = 0;
    };
    DynamicArray<float> propensities;
    DynamicArray<float> m_rates;
    DynamicArray<String> m_ids;
    DynamicArray<DynamicArray<ReqProd>> m_rules;
    module::cell::CellBase* cell;
    
    int getIndexOf(const String& id);
    
    float computePropensity(unsigned int index);
    
    void refreshPropensities(unsigned int index);
    
    void initializePropensities();
    
public:

    void operator()(simulator::Object& object, units::Duration);
    
    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, float rate);
    
    unsigned int getNumberOfReactions() const NOEXCEPT
    {
        return m_rules.size();
    }
    
    bool containsMolecule(String id);
};