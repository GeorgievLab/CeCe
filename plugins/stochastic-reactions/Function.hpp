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

/**
 * @brief Parent of all function graph nodes.
 *
 */
template <typename Return>
struct Node
{
    virtual Return eval(const Context& pointers) const = 0;
};

/**
 * @brief Operator with 2 children.
 *
 */
template<typename OperatorType>
struct OperatorTwo : public Node<typename OperatorType::result_type>
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

    OperatorTwo(NodeLeft l, NodeRight r):
    left(std::move(l)), right(std::move(r))
    {
        // Nothing to do.
    }
};

/**
 * @brief Operator with 1 children.
 *
 */
template <typename OperatorType>
struct OperatorOne : public Node<typename OperatorType::result_type>
{
    using NodeRoot = UniquePtr<Node<typename OperatorType::argument_type>>;

private:
    NodeRoot root;

public:

    typename OperatorType::result_type eval(const Context& pointers) const override
    {
        return OperatorType{}(root->eval(pointers));
    }

public:

    OperatorOne(NodeRoot r):
    root(std::move(r))
    {
        // Nothing to do.
    }
};

/**
 * @brief Leaf which uses context from cell.
 *
 */
struct IdentifierCell : public Node<RealType>
{
private:
    String m_identifier;

public:

    RealType eval(const Context& pointers) const override
    {
        return pointers.cell.getMoleculeCount(m_identifier);
    }

public:

    IdentifierCell(const String& identifier):
    m_identifier(identifier)
    {
        // Nothing to do.
    }
};

/**
 * @brief Leaf which uses context from diffusion.
 *
 */
struct IdentifierEnv : public Node<RealType>
{
private:
    String m_identifier;

public:

    RealType eval(const Context& pointers) const override
    {
        const auto id = pointers.diffusion->getSignalId(m_identifier);
        if (id != plugin::diffusion::Module::INVALID_SIGNAL_ID)
        {
            return getMolarConcentration(*pointers.diffusion, pointers.coords, id).value();
        }
        return 0;
    }

public:

    IdentifierEnv(const String& identifier):
    m_identifier(identifier)
    {
        // Nothing to do.
    }
};

/**
 * @brief Leaf with fixed value.
 *
 */
struct Amount : public Node<RealType>
{
private:
    RealType m_amount;

public:

    RealType eval(const Context& pointers) const override
    {
        return m_amount;
    }

public:

    Amount(const RealType amount):
    m_amount(amount)
    {
        // Nothing to do.
    }
};

/**
 * @brief Leaf with value from global parameters.
 *
 */
struct IdentifierPar : public Node<RealType>
{
private:
    String m_identifier;

public:

    RealType eval(const Context& pointers) const override
    {
        auto pointer = pointers.parameters.find(m_identifier);
        return pointer->second;
    }

public:

    IdentifierPar(const String& identifier):
    m_identifier(identifier)
    {
        // Nothing to do.
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
