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

template <typename T>
struct Pow
{
    constexpr T operator()(const T &lhs, const T &rhs) const
    {
        return std::pow(lhs, rhs);
    }
};

/* ************************************************************************ */

template <typename Return>
struct Node
{
    virtual Return eval(const Context& pointers) const = 0;
};

template<typename OperatorType>
struct Operator : public Node<typename OperatorType::result_type>
{
    using NodeLeft = UniquePtr<Node<typename OperatorType::first_argument_type>>;
    using NodeRight = UniquePtr<Node<typename OperatorType::second_argument_type>>;

private:
    NodeLeft left;
    NodeRight right;

public:

    typename OperatorType::result_type eval(const Context& pointers) const override
    {
        return OperatorType{}(left->eval(pointers), right->eval(pointers));
    }

public:

    Operator(NodeLeft l, NodeRight r):
    left(std::move(l)), right(std::move(r))
    {
        // Nothing to do.
    }
};

template <typename Return>
struct Function : public Node<Return>
{
private:
    SharedPtr<Node<Return>> m_root;

public:

    explicit Function<Return>(UniquePtr<Node<Return>> root)
    {
        m_root = std::move(root);
    }

    Function<Return>() = default;

public:

    Return eval(const Context& pointers) const override
    {
        return m_root->eval(pointers);
    }
};

struct IdentifierCell : public Node<RealType>
{
private:
    String identifier;

public:

    RealType eval(const Context& pointers) const override
    {
        return pointers.cell.getMoleculeCount(identifier);
    }
};

struct IdentifierEnv : public Node<RealType>
{
private:
    String identifier;

public:

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
private:
    RealType amount;

public:

    RealType eval(const Context& pointers) const override
    {
        return amount;
    }
};

struct Parameter : public Node<RealType>
{
private:
    String identifier;

public:

    RealType eval(const Context& pointers) const override
    {
        // TODO:
        return 0;
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
