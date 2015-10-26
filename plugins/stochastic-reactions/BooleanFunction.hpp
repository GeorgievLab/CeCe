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

#include "core/UniquePtr.hpp"

#include "plugins/cell/CellBase.hpp"
#include "Diffusion.hpp"

#include "Types.hpp"
#include "Context.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

class BooleanFunction
{
// Public structures:
public:

    struct Value
    {
        String m_name;
        unsigned int m_constant;

        virtual unsigned int get(const Context& pointers);
    };

    struct Operator
    {
        virtual bool eval(const Context& pointers);
    };

    struct OperatorBool : public Operator
    {
        UniquePtr<Operator> left;
        UniquePtr<Operator> right;
    };

    struct OperatorInt : public Operator
    {
        UniquePtr<Value> left;
        UniquePtr<Value> right;
    };

    struct MoleculeCount : public Value
    {
        unsigned int get(const Context& pointers) const
        {
            return pointers.cell.getMoleculeCount(m_name);
        }
    };

    struct Concetration : public Value
    {
        unsigned int get(const Context& pointers) const
        {
            const auto id = pointers.diffusion->getSignalId(m_name);
            if (id != plugin::diffusion::Module::INVALID_SIGNAL_ID)
            {
                return getMolarConcentration(*pointers.diffusion, pointers.coords, id).value();
            }
            return 0;
        }
    };

    struct Constant : public Value
    {
        unsigned int get() const
        {
            return m_constant;
        }
    };

    struct Less : public OperatorInt
    {
        bool eval(const Context& pointers) const
        {
            return left->get(pointers) < right->get(pointers);
        }
    };

    struct Greater : public OperatorInt
    {
        bool eval(const Context& pointers) const
        {
            return left->get(pointers) > right->get(pointers);
        }
    };

    struct LessEqual : public OperatorInt
    {
        bool eval(const Context& pointers) const
        {
            return left->get(pointers) <= right->get(pointers);
        }
    };

    struct GreaterEqual : public OperatorInt
    {
        bool eval(const Context& pointers) const
        {
            return left->get(pointers) <= right->get(pointers);
        }
    };

    struct Equal : public OperatorInt
    {
        bool eval(const Context& pointers) const
        {
            return left->get(pointers) == right->get(pointers);
        }
    };

    struct Is : public OperatorInt
    {
        bool eval(const Context& pointers) const
        {
            return left->get(pointers) > 0;
        }
    };

    struct And : public OperatorBool
    {
        bool eval(const Context& pointers) const
        {
            return left->eval(pointers) && right->eval(pointers);
        }
    };

    struct Or : public OperatorBool
    {
        bool eval(const Context& pointers) const
        {
            return left->eval(pointers) || right->eval(pointers);
        }
    };


// Private variables
private:

    UniquePtr<Operator> m_root;

// Public functions
public:

    bool evaluate(const Context& pointers) const
    {
        return m_root->eval(pointers);
    }

// Public constructor
public:

    BooleanFunction(Operator* node):
    m_root(node)
    {
        // Nothing to do.
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
