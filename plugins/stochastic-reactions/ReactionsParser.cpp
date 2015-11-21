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

#include "ReactionsParser.hpp"

/* ************************************************************************ */

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
    // parse conditions
    SharedPtr<Node<bool>> condition = parseCondition();
    // parse LS
    auto ids_minus = parseList();
    // check reversible reaction
    bool reversible = false;
    UniquePtr<Node<RealType>> revRate;
    if (match(TokenCode::ArrowBack, TokenCode::Less))
    {
        reversible = true;
        revRate = parseRate();
        requireNext(TokenCode::Comma);
    }
    // parse rate
    UniquePtr<Node<RealType>> rate = parseRate();
    requireNext(TokenCode::ArrowFwrd, TokenCode::Greater);
    // parse RS
    auto ids_plus = parseList();
    // expected end of reaction
    requireNext(TokenCode::Semicolon);
    // extend
    auto rules = makeRules(ids_minus, ids_plus);
    m_reactions.extend(std::move(Reaction(condition, std::move(rate), rules)));
    // extend reversible
    if (reversible)
    {
        auto revRules = makeRules(ids_minus, ids_plus);
        m_reactions.extend(std::move(Reaction(condition, std::move(revRate), revRules)));
    }
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
    if(!match(TokenCode::Function))
        return parseRelation();

    auto ptr = m_reactions.getGlobalBoolFunction(token().value);

    if (ptr == nullptr)
        throw UnknownFunctionException();

    return makeUnique<Function<bool>>(ptr);
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
            return makeUnique<OperatorTwo<std::greater_equal<RealType>>>(std::move(first), std::move(parsePlus()));
        case TokenCode::LessEqual:
            next();
            return makeUnique<OperatorTwo<std::less_equal<RealType>>>(std::move(first), std::move(parsePlus()));
        case TokenCode::Equal:
            next();
            return makeUnique<OperatorTwo<std::equal_to<RealType>>>(std::move(first), std::move(parsePlus()));
        case TokenCode::NotEqual:
            next();
            return makeUnique<OperatorTwo<std::not_equal_to<RealType>>>(std::move(first), std::move(parsePlus()));
        default:
            throw MissingOperatorException();
    }
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parseRate()
{
    auto ptr = parsePlus();
    requireNext(TokenCode::ArrowFwrd);
    return ptr;
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
    if(!match(TokenCode::Function))
        return parseUnaryMinus();

    auto ptr = m_reactions.getGlobalRealFunction(token().value);

    if (ptr == nullptr)
        throw UnknownFunctionException();

    return makeUnique<Function<RealType>>(ptr);
}

/* ************************************************************************ */

UniquePtr<Node<RealType>> ReactionsParser::parseUnaryMinus()
{
    if(match(TokenCode::Minus))
        return makeUnique<OperatorOne<std::negate<RealType>>>(std::move(parseParenthesis()));

    return parseLeaf();
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
        RealType value = strtof(token().value.c_str(), &end);
        next();
        return makeUnique<Amount>(value);
    }
    throw MissingIdentifierException();
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
            throw MissingIdentifierException{};

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
    DynamicArray<Reaction::ReqProd> array = DynamicArray<Reaction::ReqProd>(m_reactions.getMoleculeCount());

    // Conver array of requirements
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

    // Convert array of products
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

/* ************************************************************************ */
