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

#include "plugins/cell/CellBase.hpp"
#include "core/UniquePtr.hpp"
#include "Diffusion.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

class BooleanFunction
{
public:

    struct Value
    {
        union data
        {
            String name;
            unsigned int constant;
        } value;

        virtual unsigned int get(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);
    };

    struct Operator
    {
        virtual bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords);
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
        unsigned int get(const plugin::cell::CellBase& cell) const
        {
            return cell.getMoleculeCount(value.name);
        }
    };

    struct Concetration : public Value
    {
        unsigned int get(plugin::diffusion::Module* diffusion, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            const auto id = diffusion->getSignalId(value.name);
            if (id != plugin::diffusion::Module::INVALID_SIGNAL_ID)
            {
                return getMolarConcentration(*diffusion, coords, id).value();
            }
            return 0;
        }
    };

    struct Constant : public Value
    {
        unsigned int get() const
        {
            return value.constant;
        }
    };

    struct Less : public OperatorInt
    {
        bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            return left->get(diffusion, cell, coords) < right->get(diffusion, cell, coords);
        }
    };

    struct Greater : public OperatorInt
    {
        bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            return left->get(diffusion, cell, coords) > right->get(diffusion, cell, coords);
        }
    };

    struct LessEqual : public OperatorInt
    {
        bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            return left->get(diffusion, cell, coords) <= right->get(diffusion, cell, coords);
        }
    };

    struct GreaterEqual : public OperatorInt
    {
        bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            return left->get(diffusion, cell, coords) <= right->get(diffusion, cell, coords);
        }
    };

    struct Equal : public OperatorInt
    {
        bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            return left->get(diffusion, cell, coords) == right->get(diffusion, cell, coords);
        }
    };

    struct Is : public OperatorInt
    {
        bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            return left->get(diffusion, cell, coords) > 0;
        }
    };

    struct And : public OperatorBool
    {
        bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            return left->eval(diffusion, cell, coords) && right->eval(diffusion, cell, coords);
        }
    };

    struct Or : public OperatorBool
    {
        bool eval(plugin::diffusion::Module* diffusion, const plugin::cell::CellBase& cell, const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
        {
            return left->eval(diffusion, cell, coords) || right->eval(diffusion, cell, coords);
        }
    };

    UniquePtr<Operator> root;

public:

    bool evaluate(
        plugin::diffusion::Module* diffusion,
        const plugin::cell::CellBase& cell,
        const DynamicArray<plugin::diffusion::Module::Coordinate>& coords) const
    {
        return root->eval(diffusion, cell, coords);
    }

    BooleanFunction(Operator* node):
    root(node)
    {
        // Nothing to do.
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
