#pragma once

#include "simulator/Object.hpp"
#include "core/Units.hpp"
#include "core/DynamicArray.hpp"
#include "core/Grid.hpp"

class Reaction
{
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


    public:
    DynamicArray<float> m_rates;
    DynamicArray<String> m_ids;
    DynamicArray<DynamicArray<ReqProd>> m_rules;

    void operator()(simulator::Object& object, simulator::Simulation&, units::Time);

    int get_index_of(const String& id);

    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, float rate);
};
