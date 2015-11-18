/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

#include "Reactions.hpp"

/* ************************************************************************ */

namespace cece {
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
}

/* ************************************************************************ */
