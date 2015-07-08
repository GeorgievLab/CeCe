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
        bool operator ==(const ReqProd& rhs) const
        {
            return requirement == rhs.requirement && product == rhs.product;
        }
        bool operator !=(const ReqProd& rhs) const
        {
            return !operator ==(rhs);
        }

        int requirement = 0;
        int product = 0;
    };

    DynamicArray<float> m_rates;
    DynamicArray<String> m_ids;
    DynamicArray<DynamicArray<ReqProd>> m_rules;
    DynamicArray<float> propensities;
    plugin::cell::CellBase* cell;

    float computePropensity(unsigned int index);

    void refreshPropensities(unsigned int index);

    void initializePropensities();

public:
    void operator()(simulator::Object& object, simulator::Simulation&, units::Duration);

    int getIndexOf(const String& id);

    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, float rate);

    unsigned int getNumberOfReactions() const NOEXCEPT
    {
        return m_rules.size();
    }

    bool operator ==(const Reaction& rhs);

    bool areEqualReactions(const Reaction& rhs, unsigned int index1, unsigned int index2);
};
