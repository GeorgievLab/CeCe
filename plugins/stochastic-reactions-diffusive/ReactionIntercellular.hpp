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
namespace stochastic_reactions_diffusive {

/* ************************************************************************ */

struct ReqProd
{
    ReqProd() = default;
    unsigned int requirement = 0;
    unsigned int product = 0;
    bool mustnt_have = false;
    ReqProd(int req, int prod, bool flag = false) noexcept
    : requirement(req), product(prod), mustnt_have(flag)
    {
        // Nothing to do
    }
};

class ReactionIntercellular: public stochastic_reactions::Reactions<ReqProd>
{
protected:

    float computePropensity(const unsigned int index);

    void refreshPropensities(unsigned int index);

    void executeReaction(const unsigned int index);

    void changeMoleculesInEnvironment(const String& id, int change);

    void extendAbsorption(const DynamicArray<String>& ids_plus, const float rate);

    void extendExpression(const DynamicArray<String>& ids_minus, const float rate);

public:

    void operator()(simulator::Object& object, simulator::Simulation&, units::Duration);

    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const float rate);

    void addCondition(const String& id, const float condition);
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
