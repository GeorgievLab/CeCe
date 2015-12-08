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

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/SharedPtr.hpp"

#include "Types.hpp"
#include "Context.hpp"
#include "Function.hpp"

/* ************************************************************************ */

namespace cece {
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

    SharedPtr<Node<bool>> m_condition;
    SharedPtr<Node<RealType>> m_rate;
    DynamicArray<ReqProd> m_rules;

public:

    Reaction(const SharedPtr<Node<bool>> condition, const SharedPtr<Node<RealType>> rate, const DynamicArray<ReqProd>& rules):
    m_condition(condition), m_rate(rate), m_rules(rules)
    {
        // Nothing to do.
    }

// Public Accessors
public:

    inline bool evaluateCondition(const Context& pointers) const
    {
        if (m_condition == nullptr)
            return true;
        return m_condition->eval(pointers);
    }

    inline RateType evaluateRate(const Context& pointers) const
    {
        Assert(m_rate != nullptr);

        return m_rate->eval(pointers);
    }

    inline unsigned int getRequirement(unsigned int index) const noexcept
    {
        return m_rules[index].requirement;
    }

    inline unsigned int getEnvRequirement(unsigned int index) const noexcept
    {
        return m_rules[index].env_requirement;
    }

    inline unsigned int getProduct(unsigned int index) const noexcept
    {
        return m_rules[index].product;
    }

    inline unsigned int getEnvProduct(unsigned int index) const noexcept
    {
        return m_rules[index].env_product;
    }

    inline void extend() noexcept
    {
        m_rules.push_back(ReqProd());
    }
};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
