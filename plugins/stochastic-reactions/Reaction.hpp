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

#include "Types.hpp"
#include "Context.hpp"
#include "RealFunction.hpp"
#include "BooleanFunction.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

/**
 * @brief One single reaction, contains reaction rules, rate and condition.
 */
class Reaction
{

public:

    /**
    * @brief Member of reaction's stochiometric matrix.
    */
    struct ReqProd
    {
        unsigned int requirement;
        unsigned int product;
        unsigned int env_requirement;
        unsigned int env_product;

        ReqProd(unsigned int req = 0, unsigned int prod = 0, unsigned int ereq = 0, unsigned int eprod = 0) noexcept
        : requirement(req), product(prod), env_requirement(ereq), env_product(eprod)
        {
            // Nothing to do
        }
    };

// Private variables
private:

    RealFunction m_rate;
    BooleanFunction m_condition;
    DynamicArray<ReqProd> m_rules;

// Public Accessors
public:

    inline bool evaluateCondition(const Context& pointers) const noexcept
    {
        return m_condition.evaluate(pointers);
    }

    inline RateType evaluateRate(const Context& pointers) const noexcept
    {
        return m_rate.evaluate(pointers);
    }

    inline unsigned int getRequirement(unsigned int index) const
    {
        return m_rules[index].requirement;
    }

    inline unsigned int getEnvRequirement(unsigned int index) const
    {
        return m_rules[index].env_requirement;
    }

    inline unsigned int getProduct(unsigned int index) const
    {
        return m_rules[index].product;
    }

    inline unsigned int getEnvProduct(unsigned int index) const
    {
        return m_rules[index].env_product;
    }

    inline void resize (unsigned int size)
    {
        m_rules.resize(size);
    }

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
