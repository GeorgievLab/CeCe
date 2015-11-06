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
 * @brief Reaction parser token codes.
 */
enum class TokenCode
{
    Invalid,
    Identifier,
    ArrowFwrd,
    ArrowBack,
    Plus,
    Colon,
    Semicolon,
    Comma,
    And,
    Or,
    If,
    Not,
    Equal,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Null,
    Env
};

/* ************************************************************************ */

/**
 * @brief Reaction parser tokenizer.
 */
class Tokenizer
    : public BasicTokenizer<
        Tokenizer,
        BasicToken<TokenCode, String>,
        const char*
    >
{

// Public Types
public:


    // Parent type
    using ParentType = BasicTokenizer<
        Tokenizer,
        BasicToken<TokenCode, String>,
        const char*
    >;

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

        assert(isIdentifierBegin());

        token.value.push_back(value());
        next();

        while (isIdentifierRest())
        {
            token.value.push_back(value());
            next();
        }

        if (token.value == "and")
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

        if (isIdentifierBegin())
            return tokenizeIdentifier();

        switch (value())
        {
        case '-':
            next();

            if (!is('>'))
                return TokenType{};
                //fatalError("Invalid token");

            next();
            return TokenType{TokenCode::ArrowFwrd};

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

            return TokenType{TokenCode::Equal};

        case ':':
            next();
            return TokenType{TokenCode::Colon};

        case '+':
            next();
            return TokenType{TokenCode::Plus};

        case ';':
            next();
            return TokenType{TokenCode::Semicolon};

        case ',':
            next();
            return TokenType{TokenCode::Comma};
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

/* ************************************************************************ */
/**
 * @brief Base expression parser exception.
 */
class ReactionParserException: public ParserException {};

/* ************************************************************************ */

DEFINE_PARSER_EXCEPTION_BASE(MissingArrowException, ReactionParserException, "You are missing > or -> arrow.");
DEFINE_PARSER_EXCEPTION_BASE(MissingIdentifierException, ReactionParserException, "You are missing an identifier.");

/* ************************************************************************ */

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
        Reactions reactions;

        parseGlobals(reactions);

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
        auto condition = parseConditions();
        // parse LS
        auto ids_minus = parseList();
        // check reversible reaction
        bool reversible = false;
        auto revRate = ;
        if (match(TokenCode::ArrowBack, TokenCode::Less))
        {
            reversible = true;
            auto revRate = parseRate();
            requireNext(TokenCode::Comma);
        }
        // parse rate
        auto rate = parseRate();
        requireNext(TokenCode::ArrowFwrd, TokenCode::Greater);
        //parse RS
        auto ids_plus = parseList();
        // expected end of reaction
        requireNext(TokenCode::Semicolon);
        // extending
        auto rules = makeRules(ids_plus, ids_minus);
        reactions.extend(std::move(Reaction(condition, rate, rules)));
        // extending reversible
        if (reversible)
        {
            auto revRules = makeRules(ids_minus, ids_plus);
            reactions.extend(std::move(Reaction(condition, revRate, revRules)));
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

    using NodeBool = Function<bool>::Node<bool>;

    /**
     * @brief Parse conditions for conditional reactions.
     *
     * @return tuple - conditions
     */
    Function<bool> parseConditions()
    {
        if (!match(TokenCode::If))
            return {};

        auto ptr = parseOr();
        requireNext(TokenCode::Colon);
        return Function<bool>(std::move(ptr));
    }

    UniquePtr<NodeBool> parseOr()
    {
        UniquePtr<NodeBool> first = parseAnd();
        if (!match(TokenCode::Or))
            return first;

        UniquePtr<NodeBool> second = parseAnd();
        return Function<bool>::Operator<std::logical_or>(first, second);
    }

    UniquePtr<NodeBool> parseAnd()
    {

    }

    DynamicArray<Reaction::ReqProd> makeRules(DynamicArray<String> ids_plus, DynamicArray<String> ids_minus)
    {

    }

/* ************************************************************************ */
    /**
     * @brief Parse rate expression.
     *
     * @return
     */
    UniquePtr<RealFunction::Operator> parseRate()
    {
        // Alias to tokenizer range.
        const auto& tokenizerRange = getTokenizer().getRange();

        // Store rate begin
        auto begin = tokenizerRange.begin();

        // Last token was the one before rate - ArrowFwrd
        next();
        // Find arrow or semicolon
        find(TokenCode::ArrowFwrd, TokenCode::Greater, TokenCode::Semicolon);

        if (!is(TokenCode::ArrowFwrd, TokenCode::Greater))
            throw MissingArrowException();

        // Get position before token.
        auto end = tokenizerRange.begin() - token().value.size();
        auto rate = parseExpression(makeRange(begin, end), m_parameters);
        next();
        return rate;
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
