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

#include "core/Units.hpp"
#include "core/DynamicArray.hpp"
#include "core/Log.hpp"
#include "core/Exception.hpp"
#include "simulator/Object.hpp"
#include "simulator/Simulation.hpp"

#include "../cell/CellBase.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochasticreactions {

/* ************************************************************************ */

class Reaction
{
friend class ReactionParser;
friend class ReactionParserIntracellular;
friend class ReactionParserIntercellular;
friend class ReactionIntercellular;
friend class ReactionIntracellular;

protected:

    struct ReqProd
    {
        ReqProd() = default;
        unsigned int requirement = 0;
        unsigned int product = 0;
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

    // TODO: Remove, it's not a good idea to store pointer here (pass it as argument).
    plugin::cell::CellBase* cell;
    simulator::Simulation* m_simulation;

    virtual float computePropensity(unsigned int index) = 0;

    virtual void executeReaction(unsigned int index) = 0;

    void initializePropensities();

    void refreshPropensities(unsigned int index);

public:

    void operator()(simulator::Object& object, simulator::Simulation&, units::Duration);

/* ************************************************************************ */

    virtual void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, float rate) = 0;

    virtual int getIndexOf(const String& id) = 0;

    unsigned int getNumberOfReactions() const NOEXCEPT
    {
        return m_rules.size();
    }

    //bool operator ==(const Reaction& rhs);

    //virtual bool areEqualRules(const Reaction& rhs, unsigned int index1, unsigned int index2);
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
