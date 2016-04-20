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

// Declaration
#include "ReactionsParser.hpp"

// C++
#include <cstdlib>

// CeCe
#include "cece/core/UnitIo.hpp"
#include "cece/core/StringStream.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */

Reactions ReactionsParser::parse()
{
    // parse global variables and functions
    parseGlobals();
    // parse each reaction
    while (!is(TokenCode::Invalid))
    {
        parseReaction();
    }
    // return
    return m_reactions;
}

/* *********************************************************************** */

void ReactionsParser::parseReaction()
{
    // parse condition
    SharedPtr<Node<bool>> condition = parseCondition();

    // check for block condition
    bool block = false;
    if(match(TokenCode::CurlyO))
        block = true;

    do
    {
        if (match(TokenCode::If))
            throw ConditionInsideBlockException();

        // parse LS
        DynamicArray<ReactionsParser::IdEnv> ids_minus = parseList();

        // recognize reversible reaction
        UniquePtr<Node<RealType>> revRate;
        if (match(TokenCode::ArrowBack, TokenCode::Less))
        {
            revRate = parseRate();
            requireNext(TokenCode::Comma);
        }
        // require begin of rate area
        else if (!match(TokenCode::ArrowFwrd, TokenCode::Greater))
            throw MissingArrowException();

        // parse rate
        UniquePtr<Node<RealType>> rate = parseRate();

        // require end of rate area
        requireNext(TokenCode::ArrowFwrd, TokenCode::Greater);

        // parse RS
        DynamicArray<ReactionsParser::IdEnv> ids_plus = parseList();

        // expected end of reaction
        requireNext(TokenCode::Semicolon);

        // extend
        auto rules = makeRules(ids_minus, ids_plus);
        m_reactions.extend(Reaction(condition, std::move(rate), rules));

        // extend reversible
        if (revRate == nullptr)
            continue;

        auto revRules = makeRules(ids_minus, ids_plus);
        m_reactions.extend(Reaction(condition, std::move(revRate), revRules));
    }
    while (block && !match(TokenCode::CurlyC));
}

/* *********************************************************************** */

void ReactionsParser::parseGlobals()
{

}

/* *********************************************************************** */

SharedPtr<Node<bool>> ReactionsParser::parseCondition()
{
    if (!match(TokenCode::If))
        return {};

    auto ptr = parseOr();
    requireNext(TokenCode::Colon);
    return SharedPtr<Node<bool>>(std::move(ptr));
}

/* ************************************************************************ */

UniquePtr<Node<bool>> ReactionsParser::parseOr()
{
    UniquePtr<Node<bool>> first = parseAnd();
    while(match(TokenCode::Or))
    {
        UniquePtr<Node<bool>> second = parseAnd();
        first = makeUnique<OperatorTwo<std::logical_or<bool>>>(std::move(first), std::move(second));
    }
    return first;
}

/* ************************************************************************ */

UniquePtr<Node<bool>> ReactionsParser::parseAnd()
{
    UniquePtr<Node<bool>> first = parseBParenthesis();
    while(match(TokenCode::And))
    {
        UniquePtr<Node<bool>> second = parseBParenthesis();
        first = makeUnique<OperatorTwo<std::logical_and<bool>>>(std::move(first), std::move(second));
    }
    return first;
}

/* ************************************************************************ */

UniquePtr<Node<bool>> ReactionsParser::parseBParenthesis()
{
    if (!match(TokenCode::BracketO))
        return parseBoolFunction();

    UniquePtr<Node<bool>> first = parseOr();

    if (!match(TokenCode::BracketC))
        throw MissingParenthesisException();

    return first;
}

/* ************************************************************************ */

UniquePtr<Node<bool>> ReactionsParser::parseBoolFunction()
{
    if(!is(TokenCode::Function))
        return parseNot();

    auto ptr = m_reactions.getGlobalBoolFunction(token().value);

    if (ptr == nullptr)
        throw UnknownFunctionException();

    next();

    return makeUnique<Function<bool>>(ptr);
}

/* ************************************************************************ */

UniquePtr<Node<bool>> ReactionsParser::parseNot()
{
    if(!match(TokenCode::Not))
        return parseChainRelation();

    return makeUnique<OperatorOne<std::logical_not<bool>>>(std::move(parseBParenthesis()));
}

/* ************************************************************************ */

UniquePtr<Node<bool>> ReactionsParser::parseChainRelation()
{
    UniquePtr<Node<bool>> first = parseRelation();
    while(match(TokenCode::Greater, TokenCode::Less, TokenCode::GreaterEqual, TokenCode::LessEqual, TokenCode::Equal, TokenCode::NotEqual))
    {
        UniquePtr<Node<bool>> second = parseRelation();
        first = makeUnique<OperatorTwo<std::logical_and<bool>>>(std::move(first), std::move(second));
    }
    return first;
}

/* ************************************************************************ */

UniquePtr<Node<bool>> ReactionsParser::parseRelation()
{
    UniquePtr<Node<RealType>> first = parsePlus();
    switch (token().code)
    {
        case TokenCode::Greater:
            next();
            return makeUnique<OperatorTwo<std::greater<RealType>>>(std::move(first), std::move(parsePlus()));
        case TokenCode::Less:
            next();
            return makeUnique<OperatorTwo<std::less<RealType>>>(std::move(first), std::move(parsePlus()));
        case TokenCode::GreaterEqual:
            next();
            return makeUnique<OperatorTwo<GreaterEqual<RealType>>>(std::move(first), std::move(parsePlus()));
        case TokenCode::LessEqual:
            next();
            return makeUnique<OperatorTwo<LessEqual<RealType>>>(std::move(first), std::move(parsePlus()));
        case TokenCode::Equal:
            next();
            return makeUnique<OperatorTwo<Equal<RealType>>>(std::move(first), std::move(parsePlus()));
        case TokenCode::NotEqual:
            next();
            return makeUnique<OperatorTwo<NotEqual<RealType>>>(std::move(first), std::move(parsePlus()));
        default:
            return makeUnique<OperatorTwo<std::greater<RealType>>>(std::move(first), std::move(makeUnique<Amount>(RealType(0))));
    }
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parseRate()
{
    return parsePlus();
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parsePlus()
{
    UniquePtr<Node<RealType>> first = parseMultiply();
    while(is(TokenCode::Plus, TokenCode::Minus))
    {
        bool plus = is(TokenCode::Plus);
        next();
        UniquePtr<Node<RealType>> second = parseMultiply();
        if(plus)
            first = makeUnique<OperatorTwo<std::plus<RealType>>>(std::move(first), std::move(second));
        else
            first = makeUnique<OperatorTwo<std::minus<RealType>>>(std::move(first), std::move(second));
    }
    return first;
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parseMultiply()
{
    UniquePtr<Node<RealType>> first = parsePower();
    while(is(TokenCode::Multiply, TokenCode::Divide))
    {
        bool plus = is(TokenCode::Multiply);
        next();
        UniquePtr<Node<RealType>> second = parsePower();
        if(plus)
            first = makeUnique<OperatorTwo<std::multiplies<RealType>>>(std::move(first), std::move(second));
        else
            first = makeUnique<OperatorTwo<std::divides<RealType>>>(std::move(first), std::move(second));
    }
    return first;
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parsePower()
{
    UniquePtr<Node<RealType>> first = parseParenthesis();
    while(match(TokenCode::Power))
    {
        UniquePtr<Node<RealType>> second = parseParenthesis();
        first = makeUnique<OperatorTwo<Pow<RealType>>>(std::move(first), std::move(second));
    }
    return first;
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parseParenthesis()
{
    if (!match(TokenCode::BracketO))
        return parseFunction();

    UniquePtr<Node<RealType>> first = parsePlus();

    if (!match(TokenCode::BracketC))
        throw MissingParenthesisException();

    return first;
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parseFunction()
{
    if(!is(TokenCode::Function))
        return parseUnaryMinus();

    String name = token().value;
    next();

    // No parameters means predefined function
    if (match(TokenCode::BracketC))
    {
        auto ptr = m_reactions.getGlobalRealFunction(name);

        if (ptr == nullptr)
            throw UnknownFunctionException();

        return makeUnique<Function<RealType>>(ptr);
    }

    DynamicArray<UniquePtr<Node<RealType>>> nodes;

    // Read Nodes
    do
    {
        nodes.push_back(parsePlus());
    }
    while(match(TokenCode::Comma));

    // Expected closing bracket
    if (!match(TokenCode::BracketC))
        throw MissingParenthesisException();

    if (nodes.size() == 1)
    {
        if (name == "exp")
            return makeUnique<OperatorOne<Exp<RealType>>>(std::move(nodes[0]));
        else if(name == "ln")
            return makeUnique<OperatorOne<Ln<RealType>>>(std::move(nodes[0]));
        else if(name == "log10")
            return makeUnique<OperatorOne<Log10<RealType>>>(std::move(nodes[0]));
        else if(name == "log2")
            return makeUnique<OperatorOne<Log2<RealType>>>(std::move(nodes[0]));
        else if(name == "abs")
            return makeUnique<OperatorOne<Abs<RealType>>>(std::move(nodes[0]));
        else if(name == "sgn")
            return makeUnique<OperatorOne<Signum<RealType>>>(std::move(nodes[0]));
        else if(name == "sqrt")
            return makeUnique<OperatorOne<Sqrt<RealType>>>(std::move(nodes[0]));
        else if(name == "cbrt")
            return makeUnique<OperatorOne<Cbrt<RealType>>>(std::move(nodes[0]));
        else if(name == "sin")
            return makeUnique<OperatorOne<Sine<RealType>>>(std::move(nodes[0]));
        else if(name == "cos")
            return makeUnique<OperatorOne<Cosine<RealType>>>(std::move(nodes[0]));
        else if(name == "tan")
            return makeUnique<OperatorOne<Tangent<RealType>>>(std::move(nodes[0]));
        else if(name == "cot")
            return makeUnique<OperatorOne<Cotangent<RealType>>>(std::move(nodes[0]));
        else if(name == "sec")
            return makeUnique<OperatorOne<Secant<RealType>>>(std::move(nodes[0]));
        else if(name == "csc")
            return makeUnique<OperatorOne<Cosecant<RealType>>>(std::move(nodes[0]));
        else if(name == "sinh")
            return makeUnique<OperatorOne<Sinh<RealType>>>(std::move(nodes[0]));
        else if(name == "cosh")
            return makeUnique<OperatorOne<Cosh<RealType>>>(std::move(nodes[0]));
        else if(name == "tanh")
            return makeUnique<OperatorOne<Tanh<RealType>>>(std::move(nodes[0]));
        else if(name == "coth")
            return makeUnique<OperatorOne<Coth<RealType>>>(std::move(nodes[0]));
        else if(name == "sech")
            return makeUnique<OperatorOne<Sech<RealType>>>(std::move(nodes[0]));
        else if(name == "csch")
            return makeUnique<OperatorOne<Csch<RealType>>>(std::move(nodes[0]));
        else if(name == "asin")
            return makeUnique<OperatorOne<Arcsine<RealType>>>(std::move(nodes[0]));
        else if(name == "acos")
            return makeUnique<OperatorOne<Arccosine<RealType>>>(std::move(nodes[0]));
        else if(name == "atan")
            return makeUnique<OperatorOne<Arccotangent<RealType>>>(std::move(nodes[0]));
        else if(name == "acot")
            return makeUnique<OperatorOne<Arccotangent<RealType>>>(std::move(nodes[0]));
        else if(name == "asec")
            return makeUnique<OperatorOne<Arcsecant<RealType>>>(std::move(nodes[0]));
        else if(name == "acsc")
            return makeUnique<OperatorOne<Arccosecant<RealType>>>(std::move(nodes[0]));
        else if(name == "asinh")
            return makeUnique<OperatorOne<Arcsinh<RealType>>>(std::move(nodes[0]));
        else if(name == "acosh")
            return makeUnique<OperatorOne<Arccosh<RealType>>>(std::move(nodes[0]));
        else if(name == "atanh")
            return makeUnique<OperatorOne<Arctanh<RealType>>>(std::move(nodes[0]));
        else if(name == "acoth")
            return makeUnique<OperatorOne<Arccoth<RealType>>>(std::move(nodes[0]));
        else if(name == "asech")
            return makeUnique<OperatorOne<Arcsech<RealType>>>(std::move(nodes[0]));
        else if(name == "acsch")
            return makeUnique<OperatorOne<Arccsch<RealType>>>(std::move(nodes[0]));
        else if(name == "gamma")
            return makeUnique<OperatorOne<Gamma<RealType>>>(std::move(nodes[0]));
    }
    else if (nodes.size() == 2)
    {
        if (name == "pow")
            return makeUnique<OperatorTwo<Pow<RealType>>>(std::move(nodes[0]), std::move(nodes[1]));
        else if (name == "log")
            return makeUnique<OperatorTwo<Log<RealType>>>(std::move(nodes[0]), std::move(nodes[1]));
    }
    else if (nodes.size() == 3)
    {
        if (name == "hill")
            return makeUnique<OperatorThree<Hill<RealType>>>(std::move(nodes[0]), std::move(nodes[1]), std::move(nodes[2]));
    }

    throw UnknownFunctionException();
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parseUnaryMinus()
{
    if(!match(TokenCode::Minus))
        return parseLeaf();

    return makeUnique<OperatorOne<std::negate<RealType>>>(std::move(parseParenthesis()));
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parseLeaf()
{
    if(match(TokenCode::Env))
    {
        require(TokenCode::Identifier);
        String identifier = token().value;
        next();
        return makeUnique<IdentifierEnv>(identifier);
    }
    if(match(TokenCode::Parameter))
    {
        require(TokenCode::Identifier);
        String identifier = token().value;
        next();
        return makeUnique<IdentifierPar>(identifier);
    }
    if(is(TokenCode::Identifier))
    {
        String identifier = token().value;
        next();
        return makeUnique<IdentifierCell>(identifier);
    }
    if(is(TokenCode::Number))
    {
        char* end;
        RealType value = strtod(token().value.c_str(), &end);
        next();
        return makeUnique<Amount>(value);
    }
    if(is(TokenCode::Units))
    {
        InStringStream iss(token().value);
        next();
        return makeUnique<Amount>(core::units::parse(iss));
    }
    throw MissingNumberException();
}

/* ************************************************************************ */

DynamicArray<ReactionsParser::IdEnv> ReactionsParser::parseList()
{
    DynamicArray<IdEnv> array;
    do
    {
        if (match(TokenCode::Null)) // I decided to ignore "Null must be alone" syntax error.
            continue;

        bool environment = match(TokenCode::Env);

        if (!is(TokenCode::Identifier))
            throw MissingIdentifierException();

        array.push_back(IdEnv(token().value, environment));
        next();
    }
    while (match(TokenCode::Plus));
    return array;
}

/* ************************************************************************ */

DynamicArray<Reaction::ReqProd> ReactionsParser::makeRules(DynamicArray<IdEnv> ids_minus, DynamicArray<IdEnv> ids_plus)
{
    using ReqProd = Reaction::ReqProd;

    // Initialize array
    DynamicArray<ReqProd> array(m_reactions.getMoleculeCount());

    // array of requirements
    for (auto &local : ids_minus)
    {
        auto index = m_reactions.getMoleculeIndex(local.id);
        if (index == array.size())
        {
            if(local.env)
                array.push_back(ReqProd(0,0,1,0));
            else
                array.push_back(ReqProd(1,0,0,0));
        }
        else
        {
            if(local.env)
                array[index].env_requirement += 1;
            else
                array[index].requirement += 1;
        }
    }

    // array of products
    for (auto &local : ids_plus)
    {
        auto index = m_reactions.getMoleculeIndex(local.id);
        if (index == array.size())
        {
            if(local.env)
                array.push_back(ReqProd(0,0,0,1));
            else
                array.push_back(ReqProd(0,1,0,0));
        }
        else
        {
            if(local.env)
                array[index].env_product += 1;
            else
                array[index].product += 1;
        }
    }

    // Return
    return array;
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
