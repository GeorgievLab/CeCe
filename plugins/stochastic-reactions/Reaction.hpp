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

#include "simulator/Object.hpp"
#include "core/Units.hpp"
#include "core/DynamicArray.hpp"
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "../cell/CellBase.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */
    
class Reaction
{
private:

    struct ReqProd
    {
        ReqProd() = default;
        int requirement = 0;
        int product = 0;
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
    };
    DynamicArray<float> m_rates;
    DynamicArray<String> m_ids;
    DynamicArray<DynamicArray<ReqProd>> m_rules;
    DynamicArray<float> propensities;
    plugin::cell::CellBase* cell;

    virtual float computePropensity(unsigned int index);
    
    virtual void executeReaction(unsigned int index);

    void refreshPropensities(unsigned int index);

    void initializePropensities();

public:

    void operator()(simulator::Object& object, simulator::Simulation&, units::Duration);

/* ************************************************************************ */

    virtual void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, float rate);
    
    int getIndexOf(const String& id);

    unsigned int getNumberOfReactions() const NOEXCEPT
    {
        return m_rules.size();
    }

    bool operator ==(const Reaction& rhs);

    bool areEqualRules(const Reaction& rhs, unsigned int index1, unsigned int index2);
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */