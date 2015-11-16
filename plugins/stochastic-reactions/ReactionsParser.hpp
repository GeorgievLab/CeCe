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

#include "core/DynamicArray.hpp"
#include "core/ExpressionParser.hpp"
#include "core/Log.hpp"
#include "core/Tokenizer.hpp"
#include "core/Parser.hpp"
#include "core/Tuple.hpp"

#include "Types.hpp"
#include "Reactions.hpp"

/* ************************************************************************ */

namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */
/**
 * @brief Base expression parser exception.
 */
class ReactionParserException: public ParserException {};

/* ************************************************************************ */

DEFINE_PARSER_EXCEPTION_BASE(MissingArrowException, ReactionParserException, "You are missing > or -> arrow.");
DEFINE_PARSER_EXCEPTION_BASE(MissingIdentifierException, ReactionParserException, "You are missing an identifier.");
DEFINE_PARSER_EXCEPTION_BASE(MissingOperatorException, ReactionParserException, "You are missing an operator.");
DEFINE_PARSER_EXCEPTION_BASE(MissingParenthesisException, ReactionParserException, "You are missing a closing bracket.");
DEFINE_PARSER_EXCEPTION_BASE(InvalidOperatorException, ReactionParserException, "I dont know this operator.");

/* ************************************************************************ */

/**
 * @brief Reaction parser token codes.
 */
enum class TokenCode
{
    Invalid,
    Identifier,
    Number,
    ArrowFwrd,
    ArrowBack,
    Plus,
    Minus,
    Multiply,
    Divide,
    Power,
    Colon,
    Semicolon,
    Comma,
    And,
    Or,
    If,
    Not,
    Equal,
    NotEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    Null,
    Env,
    BracketO,
    BracketC,
    Function
};

/* ************************************************************************ */

/**
 * @brief Reaction parser tokenizer.
 */
class Tokenizer
    : public BasicTokenizer<
        Tokenizer,
        BasicToken<TokenCode, String>,
        const char*>
{

// Public Types
public:

    // Parent type
    using ParentType = BasicTokenizer<
        Tokenizer,
        BasicToken<TokenCode, String>,
        const char*>;

    /// Token type
    using TokenType = typename ParentType::TokenType;


// Public Ctors & Dtors
public:

    using ParentType::ParentType;

// Public Operation
public:

    /**
     * @brief Tokenize identifier.
     *
     * @return
     */
    TokenType tokenizeIdentifier() noexcept
    {
        TokenType token{TokenCode::Identifier};

        token.value.push_back(value());
        next();

        while (isIdentifierRest())
        {
            token.value.push_back(value());
            next();
        }

        if (value() == '(')
        {
            token.code = TokenCode::Function;
            next();
        }
        else if (token.value == "and")
            token.code = TokenCode::And;
        else if (token.value == "or")
            token.code = TokenCode::Or;
        else if (token.value == "if")
            token.code = TokenCode::If;
        else if (token.value == "not")
            token.code = TokenCode::Not;
        else if (token.value == "null")
            token.code = TokenCode::Null;
        else if (token.value == "env")
            token.code = TokenCode::Env;

        return token;
    }


    /**
     * @brief Tokenize source.
     *
     * @return
     */
    TokenType tokenize()
    {
        // Skip whitespaces
        while (isRange('\1', ' '))
            next();

        // Skip comments
        while (is('#'))
        {
            find('\n', '\r');
            while (isRange('\1', ' '))
                next();
        }

        if(isDigit())
        {
            TokenType token{TokenCode::Number};

            do
            {
                token.value.push_back(value());
                next();
            }
            while(isDigit());

            return token;
        }

        if (isIdentifierBegin())
            return tokenizeIdentifier();

        switch (value())
        {
        case '-':
            next();

            if (match('>'))
                return TokenType{TokenCode::ArrowFwrd};

            throw TokenType{TokenCode::Minus};

        case '>':
            next();

            if (match('='))
                return TokenType{TokenCode::GreaterEqual};

            return TokenType{TokenCode::Greater};

        case '<':
            next();

            if (match('-'))
                return TokenType{TokenCode::ArrowBack};
            if (match('='))
                return TokenType{TokenCode::LessEqual};

            return TokenType{TokenCode::Less};

        case '=':
            next();

            if (match('<'))
                return TokenType{TokenCode::LessEqual};
            if (match('>'))
                return TokenType{TokenCode::GreaterEqual};
            if (match('='))
                return TokenType{TokenCode::Equal};

            return TokenType{TokenCode::Equal};

        case '!':
            next();

            if (match('='))
                return TokenType{TokenCode::NotEqual};

            throw InvalidOperatorException();

        case ':':
            next();
            return TokenType{TokenCode::Colon};

        case '+':
            next();
            return TokenType{TokenCode::Plus};

        case '*':
            next();
            return TokenType{TokenCode::Multiply};

        case '/':
            next();
            return TokenType{TokenCode::Divide};

        case '^':
            next();
            return TokenType{TokenCode::Power};

        case ';':
            next();
            return TokenType{TokenCode::Semicolon};

        case ',':
            next();
            return TokenType{TokenCode::Comma};

        case '(':
            next();
            return TokenType{TokenCode::BracketO};

        case ')':
            next();
            return TokenType{TokenCode::BracketC};
        }

        // In case of invalid character, generate invalid token, but
        // move to next character, so we don't keep trying to create
        // a token from same character
        next();

        return TokenType{};
    }

// Protected Operation
protected:

    /**
     * @brief Check if current value is digit.
     *
     * @return
     */
    bool isDigit() const noexcept
    {
        return isRange('0', '9');
    }

    /**
     * @brief Check if current value is identifier begin.
     *
     * @return
     */
    bool isIdentifierBegin() const noexcept
    {
        return isRange('a', 'z') || isRange('A', 'Z');
    }

    /**
     * @brief Check if current value is identifier rest.
     *
     * @return
     */
    bool isIdentifierRest() const noexcept
    {
        return isIdentifierBegin() || isDigit() || is('_');
    }

};

class ReactionsParser : public BasicParser<ReactionsParser, Tokenizer>
{

// Public Types
public:

    // Parent type.
    using ParentType = BasicParser<ReactionsParser, Tokenizer>;

// Public Ctors & Dtors
public:

/* ************************************************************************ */
    /**
     * @brief Constructor.
     *
     * @param code       Source code to parse.
     * @param parameters Reactions parameters.
     */
    explicit ReactionsParser(const String& code, const Map<String, float>& parameters = Map<String, float>{}) noexcept
        : ParentType(code.c_str(), code.c_str() + code.size()), m_parameters(parameters)
    {
        // Nothing to do.
    }

/* ************************************************************************ */
    /**
     * @brief Parse source string into reactions.
     *
     * @return
     */
    Reactions parse()
    {
        // Initialize Reaction
        Reactions reactions;
        // Parse global variables and functions
        parseGlobals(reactions);
        // Parse each reaction
        while (!is(TokenCode::Invalid))
        {
            parseReaction(reactions);
        }
        return reactions;
    }

/* ************************************************************************ */
    void parseReaction(Reactions& reactions)
    {
        // parse conditions
        UniquePtr<Node<bool>> condition = parseCondition();
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
        //parse RS
        auto ids_plus = parseList();
        // expected end of reaction
        requireNext(TokenCode::Semicolon);
        // extending
        auto rules = makeRules(ids_plus, ids_minus);
        reactions.extend(std::move(Reaction(std::move(condition), std::move(rate), rules)));
        // extending reversible
        if (reversible)
        {
            auto revRules = makeRules(ids_minus, ids_plus);
            reactions.extend(std::move(Reaction(std::move(condition), std::move(revRate), revRules)));
        }
    }

/* *********************************************************************** */

protected:
    // Use parent's member functions
    using ParentType::is;
    using ParentType::next;
    using ParentType::match;
    using ParentType::require;
    using ParentType::requireNext;
    using ParentType::getTokenizer;
    using ParentType::token;
    using ParentType::find;

/* ************************************************************************ */

    void parseGlobals(Reactions& reactions)
    {

    }

/* ************************************************************************ */
    /**
     * @brief Parse conditions for conditional reactions.
     *
     * @return pointer to node which is evaulated for true/false
     */
    UniquePtr<Node<bool>> parseCondition()
    {
        if (!match(TokenCode::If))
            return {};

        auto ptr = parseOr();
        requireNext(TokenCode::Colon);
        return ptr;
    }

    UniquePtr<Node<bool>> parseOr()
    {
        UniquePtr<Node<bool>> first = parseAnd();
        while(match(TokenCode::Or))
        {
            UniquePtr<Node<bool>> second = parseAnd();
            first = makeUnique<Operator<std::logical_or<bool>>>(std::move(first), std::move(second));
        }
        return first;
    }

    UniquePtr<Node<bool>> parseAnd()
    {
        UniquePtr<Node<bool>> first = parseBParenthesis();
        while(match(TokenCode::And))
        {
            UniquePtr<Node<bool>> second = parseBParenthesis();
            first = makeUnique<Operator<std::logical_and<bool>>>(std::move(first), std::move(second));
        }
        return first;
    }

    UniquePtr<Node<bool>> parseBParenthesis()
    {
        if (!match(TokenCode::BracketO))
            return parseBoolFunction();

        UniquePtr<Node<bool>> first = parseOr();

        if (!match(TokenCode::BracketC))
            throw MissingParenthesisException();

        return first;
    }

    UniquePtr<Node<bool>> parseBoolFunction()
    {
        if(!match(TokenCode::Function))
            return parseRelation();

        //auto distance = std::find() prohledat preddefinovane fce
        // return ptr na fci
    }

    UniquePtr<Node<bool>> parseChainRelation()
    {
        UniquePtr<Node<bool>> first = parseRelation();
        while(match(TokenCode::Greater, TokenCode::Less, TokenCode::GreaterEqual, TokenCode::LessEqual, TokenCode::Equal, TokenCode::NotEqual))
        {
            UniquePtr<Node<bool>> second = parseRelation();
            first = makeUnique<Operator<std::logical_and<bool>>>(std::move(first), std::move(second));
        }
        return first;
    }

    UniquePtr<Node<bool>> parseRelation()
    {
        UniquePtr<Node<RealType>> first = parsePlus();
        switch (token().code)
        {
            case TokenCode::Greater:
                next();
                return makeUnique<Operator<std::greater<bool>>>(std::move(first), std::move(parsePlus()));
            case TokenCode::Less:
                next();
                return makeUnique<Operator<std::less<bool>>>(std::move(first), std::move(parsePlus()));
            case TokenCode::GreaterEqual:
                next();
                return makeUnique<Operator<std::greater_equal<bool>>>(std::move(first), std::move(parsePlus()));
            case TokenCode::LessEqual:
                next();
                return makeUnique<Operator<std::less_equal<bool>>>(std::move(first), std::move(parsePlus()));
            case TokenCode::Equal:
                next();
                return makeUnique<Operator<std::equal_to<bool>>>(std::move(first), std::move(parsePlus()));
            case TokenCode::NotEqual:
                next();
                return makeUnique<Operator<std::not_equal_to<bool>>>(std::move(first), std::move(parsePlus()));
            default:
                throw MissingOperatorException();
        }
    }

/* ************************************************************************ */
    /**
     * @brief Make rules from Strings
     *
     * @return array of ReqProd
     */
    DynamicArray<Reaction::ReqProd> makeRules(DynamicArray<String> ids_plus, DynamicArray<String> ids_minus)
    {

    }

/* ************************************************************************ */
    /**
     * @brief Parse rate expression.
     *
     * @return
     */
    UniquePtr<Node<RealType>> parseRate()
    {
        auto ptr = parsePlus();
        requireNext(TokenCode::ArrowFwrd);
        return ptr;
    }

    UniquePtr<Node<RealType>> parsePlus()
    {
        UniquePtr<Node<RealType>> first = parseMultiply();
        while(is(TokenCode::Plus, TokenCode::Minus))
        {
            bool plus = is(TokenCode::Plus);
            next();
            UniquePtr<Node<RealType>> second = parseMultiply();
            if(plus)
                first = makeUnique<Operator<std::plus<RealType>>>(std::move(first), std::move(second));
            else
                first = makeUnique<Operator<std::minus<RealType>>>(std::move(first), std::move(second));
        }
        return first;
    }

    UniquePtr<Node<RealType>> parseMultiply()
    {
        UniquePtr<Node<RealType>> first = parsePower();
        while(is(TokenCode::Multiply, TokenCode::Divide))
        {
            bool plus = is(TokenCode::Multiply);
            next();
            UniquePtr<Node<RealType>> second = parsePower();
            if(plus)
                first = makeUnique<Operator<std::multiplies<RealType>>>(std::move(first), std::move(second));
            else
                first = makeUnique<Operator<std::divides<RealType>>>(std::move(first), std::move(second));
        }
        return first;
    }

    UniquePtr<Node<RealType>> parsePower()
    {
        UniquePtr<Node<RealType>> first = parseParenthesis();
        while(match(TokenCode::Power))
        {
            UniquePtr<Node<RealType>> second = parseParenthesis();
            first = makeUnique<Operator<Pow<RealType>>>(std::move(first), std::move(second));
        }
        return first;
    }

    UniquePtr<Node<RealType>> parseParenthesis()
    {
        if (!match(TokenCode::BracketO))
            return parseFunction();

        UniquePtr<Node<RealType>> first = parsePlus();

        if (!match(TokenCode::BracketC))
            throw MissingParenthesisException();

        return first;
    }

    UniquePtr<Node<RealType>> parseFunction()
    {
        if(!match(TokenCode::Function))
            return parseConstant();

        // prohledat fce v vratit
    }

    UniquePtr<Node<RealType>> parseVariable()
    {
        if(match(TokenCode::))
    }

/* ************************************************************************ */
    /**
     * @brief Parse list of identifiers (molecule names).
     *
     * @return
     */
    DynamicArray<String> parseList()
    {
        DynamicArray<String> array;
        do
        {
            if (!is(TokenCode::Identifier))
                throw MissingIdentifierException{};
            array.push_back(token().value);
            next();
        }
        while (match(TokenCode::Plus));
        return array;
    }


// Private Data Members
private:


    /// Parser parameters.
    const Map<String, float>& m_parameters;

};

/* ************************************************************************ */

/**
 * @brief Parse reactions code.
 *
 * @param code       Reactions code.
 * @param parameters Map of variables.
 *
 * @return Result reactions.
 */
inline Reactions parseReactions(const String& code, const Map<String, float>& parameters = Map<String, float>{})
{
    return ReactionsParser(code, parameters).parse();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
