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

#include "../stochastic-reactions/Reactions.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

struct ReqProd
{
    ReqProd() = default;
    unsigned int requirement = 0;
    unsigned int product = 0;
    unsigned int env_requirement = 0;
    unsigned int env_product = 0;
    bool mustnt_have = false;
    ReqProd(int req, int prod, int e_req = 0, int e_prod = 0, bool flag = false) noexcept
    : requirement(req), product(prod), env_requirement(e_req), env_product(e_prod), mustnt_have(flag)
    {
        // Nothing to do
    }
};

class IntercellularReactions: public stochastic_reactions::Reactions<ReqProd>
{
protected:

    NumberType computePropensity(const unsigned int index, const plugin::cell::CellBase& cell, simulator::Simulation& simulation);

    void refreshPropensities(unsigned int index, const plugin::cell::CellBase& cell);

    void executeReaction(const unsigned int index, plugin::cell::CellBase& cell);

    void changeMoleculesInEnvironment(const String& id, int change);

    void extendAbsorption(const DynamicArray<String>& ids_plus, const float rate);

    void extendExpression(const DynamicArray<String>& ids_minus, const float rate);

public:

    void operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration step);

    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const NumberType rate);

    void addCondition(const String& id, const unsigned int condition);
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
