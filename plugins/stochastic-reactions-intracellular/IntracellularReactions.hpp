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

#include "Reactions.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

struct ReqProd
{
    ReqProd() = default;
    unsigned int requirement = 0;
    unsigned int product = 0;
    bool less = false;
    ReqProd(int req, int prod, bool flag = false) noexcept
    : requirement(req), product(prod), less(flag)
    {
        // Nothing to do
    }
};

/* ************************************************************************ */

class IntracellularReactions: public Reactions<ReqProd>
{

protected:

    PropensityType computePropensity(const unsigned int index, const plugin::cell::CellBase& cell);

    void initializePropensities(const plugin::cell::CellBase& cell);

    void refreshPropensities(const unsigned int index, const plugin::cell::CellBase& cell);

    void executeReaction(const unsigned int index, plugin::cell::CellBase& cell);

public:

    void operator()(simulator::Object& object, simulator::Simulation&, units::Duration step);

    void extend(const DynamicArray<String>& ids_plus, const DynamicArray<String>& ids_minus, const RateType rate);
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
