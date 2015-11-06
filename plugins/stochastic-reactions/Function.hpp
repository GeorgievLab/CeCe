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
#include "core/SharedPtr.hpp"

#include "plugins/cell/CellBase.hpp"
#include "Diffusion.hpp"

#include "Types.hpp"
#include "Context.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

template <typename Mode>
class Function
{
// Public structures:
public:

    template <typename Return>
    struct Node
    {
        virtual Return eval(const Context& pointers) const = 0;
    };

    template<typename OperatorType>
    struct Operator : public Node<typename OperatorType::result_type>
    {
        UniquePtr<Node<typename OperatorType::first_argument_type>> left;
        UniquePtr<Node<typename OperatorType::second_argument_type>> right;

        typename OperatorType::result_type eval(const Context& pointers) const
        {
            return OperatorType{}(left->eval(pointers), right->eval(pointers));
        }
    };

    struct IdentifierCell : public Node<RealType>
    {
        String identifier;

        RealType eval(const Context& pointers) const override
        {
            return pointers.cell.getMoleculeCount(identifier);
        }
    };

    struct IdentifierEnv : public Node<RealType>
    {
        String identifier;

        RealType eval(const Context& pointers) const override
        {
            const auto id = pointers.diffusion->getSignalId(identifier);
            if (id != plugin::diffusion::Module::INVALID_SIGNAL_ID)
            {
                return getMolarConcentration(*pointers.diffusion, pointers.coords, id).value();
            }
            return 0;
        }
    };

    struct Amount : public Node<RealType>
    {
        RealType amount;

        RealType eval(const Context& pointers) const override
        {
            return amount;
        }
    };

    struct Parameter : public Node<RealType>
    {
        String identifier;

        RealType eval(const Context& pointers) const override
        {
            // TODO:
            return 0;
        }
    };

    template <typename ReturnType>
    struct InnerFunction : public Node<ReturnType>
    {
        Function<ReturnType> function;

        ReturnType get(const Context& pointers) const override
        {
            return function.evaluate(pointers);
        }
    };

// Private variables
private:

    SharedPtr<Node<Mode>> m_root;

// Public contructors
public:

    explicit Function<Mode>(UniquePtr<Node<Mode>> root)
    {
        m_root = std::move(root);
    }

    Function<Mode>() = default;

// Public functions
public:

   Mode evaluate(const Context& pointers) const
    {
        return m_root->eval(pointers);
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
