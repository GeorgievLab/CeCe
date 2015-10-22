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

#include "../cell/CellBase.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

class BooleanFunction
{
    struct Node
    {
        virtual bool eval();
    };

    struct Value
    {
        unsigned int value;
    };

    struct Root
    {
        UniquePtr<Node> left;
        UniquePtr<Node> right;
    };

    struct MoleculeCount : public Value
    {
        unsigned int eval(const plugin::cell::CellBase& cell)
        {
            return cell.getMoleculeCount(getMoleculeName[value]);
        }
    };

    struct Concetration : public Value
    {
        unsigned int get()
        {
            return 5;
        }
    };

    struct Constant : public Value
    {
        unsigned int get()
        {
            return value;
        }
    };

    struct Less : public Root
    {
        bool eval() const override
        {
            return left->eval() < right->eval();
        }
    };

    struct Greater : public Root
    {
        bool eval() const override
        {
            return left->eval() > R->eval();
        }
    };

    struct LessEqual : public Root
    {
        bool eval() const override
        {
            return left->eval() <= R->eval();
        }
    };

    struct GreaterEqual : public Root
    {
        bool eval() const override
        {
            return left->eval() <= R->eval();
        }
    };

    struct Equal : public Node
    {
        Value left;
        Value R;

        bool evaulate()
        {
            return left.get() == R.get();
        }
    };

    struct Is : public Node
    {
        bool eval()
        {
            return left.get() > 0;
        }
    };

    struct And : public Node
    {
        Node left;
        Node R;

        bool evaulate()
        {
            return left.evaulate() && R.evaulate();
        }
    };

    struct Or : public Node
    {
        Node left;
        Node R;

        bool evaulate()
        {
            return left.evaulate() || R.evaulate();
        }
    };

    Node root;

public:

    bool evaulateFunction(const plugin::cell::CellBase& cell)
    {
        return root.evaulate(const plugin::cell::CellBase& cell);
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
