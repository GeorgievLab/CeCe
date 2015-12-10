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

// CeCe
#include "cece/core/ExpressionParser.hpp"
#include "cece/core/DynamicArray.hpp"
#include "cece/core/Tokenizer.hpp"
#include "cece/core/Parser.hpp"
#include "cece/core/Tuple.hpp"
#include "cece/core/Pair.hpp"
#include "cece/core/Map.hpp"
#include "cece/core/Log.hpp"

#include "Reactions.hpp"
#include "Functors.hpp"
#include "Types.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace stochastic_reactions {

/* ************************************************************************ */
/**
 * @brief Base expression parser exception.
 */
class ReactionParserException: public ParserException {};

/* ************************************************************************ */

DEFINE_PARSER_EXCEPTION_BASE(MissingArrowException, ReactionParserException, "Expected > or -> arrow.");
DEFINE_PARSER_EXCEPTION_BASE(MissingIdentifierException, ReactionParserException, "Expected identifier.");
DEFINE_PARSER_EXCEPTION_BASE(MissingNumberException, ReactionParserException, "Expected number.");
DEFINE_PARSER_EXCEPTION_BASE(MissingOperatorException, ReactionParserException, "Expected operator.");
DEFINE_PARSER_EXCEPTION_BASE(MissingParenthesisException, ReactionParserException, "Expected closing bracket.");
DEFINE_PARSER_EXCEPTION_BASE(UnknownOperatorException, ReactionParserException, "Unknown operator.");
DEFINE_PARSER_EXCEPTION_BASE(UnknownFunctionException, ReactionParserException, "Unknown function name.");
DEFINE_PARSER_EXCEPTION_BASE(IncorrectNumberFormatException, ReactionParserException, "Incorrect number format.");

/* ************************************************************************ */

/**
 * @brief Reaction parser token codes.
 */
enum class TokenCode
{
    Invalid,
    Identifier,
    Number,
    Units,
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
    Parameter,
    BracketO,
    BracketC,
    CurlyO,
    CurlyC,
    Function
};

/* ************************************************************************ */

/**
 * @brief Reaction parser tokenizer.
 */
class Tokenizer : public BasicTokenizer<Tokenizer, BasicToken<TokenCode, String>, const char*>
{

// Public Types
public:

    // Parent type
    using ParentType = BasicTokenizer<Tokenizer, BasicToken<TokenCode, String>, const char*>;

    /// Token type
    using TokenType = typename ParentType::TokenType;


// Public Ctors & Dtors
public:

    using ParentType::ParentType;

// Public tokenizer operations
public:

    /**
     * @brief Tokenize identifier.
     *
     * @return
     */
    TokenType tokenizeIdentifier() noexcept
    {
        // initialize token
        TokenType token{TokenCode::Identifier};
        // fill the string
        do
        {
            token.value.push_back(value());
            next();
        }
        while (isIdentifierRest());

        skipWhitespace();

        // check if string is keyword
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
        else if (token.value == "par")
            token.code = TokenCode::Parameter;
        // ...or function name
        else if (value() == '(')
        {
            token.code = TokenCode::Function;
            next();
        }

        return token;
    }

    TokenType tokenizeNumber()
    {
        TokenType token{TokenCode::Number};
        // fill
        do
        {
            token.value.push_back(value());
            next();
        }
        while (isDigit());

        // check for decimal point
        if (is('.'))
        {
            // add decimal point
            token.value.push_back(value());
            next();
            // require at least one digit after decimal point
            if (!isDigit())
                throw IncorrectNumberFormatException();
            // fill
            do
            {
                token.value.push_back(value());
                next();
            }
            while (isDigit());
        }

        // check for exponent sign
        if (is('e') || is('E'))
        {
            // add exponent sign
            token.value.push_back(value());
            next();
            // optional sign character
            if (is('-') || is('+'))
            {
                token.value.push_back(value());
                next();
            }
            // require at least one digit after exponent sign
            if (!isDigit())
                throw IncorrectNumberFormatException();
            // fill
            do
            {
                token.value.push_back(value());
                next();
            }
            while (isDigit());
        }

        // units appendix
        if (isIdentifierBegin())
        {
            token.code = TokenCode::Units;
            do
            {
                token.value.push_back(value());
                next();
            }
            while (isIdentifierBegin());
            if (is('/'))
            {
                do
                {
                    token.value.push_back(value());
                    next();
                }
                while (isIdentifierBegin());
            }
        }

        return token;
    }

    /**
     * @brief Main tokenize function.
     *
     * @return
     */
    TokenType tokenize()
    {
        skipWhitespace();

        // Skip comments
        while (is('#'))
        {
            find('\n', '\r');
            while (isRange('\1', ' '))
                next();
        }

        // Number
        if(isDigit())
            return tokenizeNumber();

        // Identifier
        if (isIdentifierBegin())
            return tokenizeIdentifier();

        // Operators
        switch (value())
        {
        case '-':
            next();
            if (match('>'))
                return TokenType{TokenCode::ArrowFwrd};
            return TokenType{TokenCode::Minus};
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
            throw UnknownOperatorException();
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
        case '{':
            next();
            return TokenType{TokenCode::CurlyO};
        case '}':
            next();
            return TokenType{TokenCode::CurlyC};
        }
        // move to next character and return invalid
        next();
        return TokenType{};
    }

// Protected Operations
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
     * @brief Check if current value is whitespace.
     *
     * @return
     */
    bool isWhitespace() const noexcept
    {
        return isRange('\1' , ' ');
    }

    /**
     * @brief Moves iterator beyond whitespace.
     *
     * @return
     */
    void skipWhitespace() noexcept
    {
        while (isWhitespace())
        {
            next();
        }
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
/* ************************************************************************ */

/**
 * @brief ReactionParser class
 *
 */
class ReactionsParser : public BasicParser<ReactionsParser, Tokenizer>
{

// Private members
private:

    Reactions m_reactions;

// Public Types
public:

    // Parent type.
    using ParentType = BasicParser<ReactionsParser, Tokenizer>;

// Private tokenizer functions
private:

    using ParentType::is;
    using ParentType::next;
    using ParentType::match;
    using ParentType::require;
    using ParentType::requireNext;
    using ParentType::getTokenizer;
    using ParentType::token;
    using ParentType::find;

/* ************************************************************************ */

public:

    /**
     * @brief Constructor.
     *
     * @param code       Source code to parse.
     * @param parameters Reactions parameters.
     */
    explicit ReactionsParser(const String& code) noexcept
        : ParentType(code.c_str(), code.c_str() + code.size())
    {
        // Nothing to do.
    }

/* ************************************************************************ */

public:

    /**
     * @brief Parse source string into reactions.
     *
     * @return
     */
    Reactions parse();

private:

    /**
     * @brief Parse one individual reaction.
     *
     * @return
     */
    void parseReaction();

    /**
     * @brief Parse global functions.
     *
     * @return
     */
    void parseGlobals();

/* ************************************************************************ */

private:

    /**
     * @brief Parse condition for conditional reactions.
     *
     * @return pointer to node which is evaulated for true/false
     */
    SharedPtr<Node<bool>> parseCondition();

    UniquePtr<Node<bool>> parseOr();

    UniquePtr<Node<bool>> parseAnd();

    UniquePtr<Node<bool>> parseBParenthesis();

    UniquePtr<Node<bool>> parseBoolFunction();

    UniquePtr<Node<bool>> parseNot();

    UniquePtr<Node<bool>> parseChainRelation();

    UniquePtr<Node<bool>> parseRelation();

/* ************************************************************************ */

private:

    /**
     * @brief Parse rate expression.
     *
     * @return
     */
    UniquePtr<Node<RealType>> parseRate();

    UniquePtr<Node<RealType>> parsePlus();

    UniquePtr<Node<RealType>> parseMultiply();

    UniquePtr<Node<RealType>> parsePower();

    UniquePtr<Node<RealType>> parseParenthesis();

    UniquePtr<Node<RealType>> parseFunction();

    UniquePtr<Node<RealType>> parseUnaryMinus();

    UniquePtr<Node<RealType>> parseLeaf();

/* ************************************************************************ */

private:

    /**
     * @brief Container for molecule name and its flag showing whether its environmental or not.
     *
     */
    struct IdEnv
    {
        String id;
        bool env;

        IdEnv (const String i, const bool e):
        id(i), env(e)
        {
            // Nothing to do.
        }
    };

    /**
     * @brief Parse list of identifiers (molecule names).
     *
     * @return Array of identifiers.
     */
    DynamicArray<IdEnv> parseList();

    /**
    * @brief Create ReqProd array from string arrays.
    *
    * @return Reaction rule.
    */
    DynamicArray<Reaction::ReqProd> makeRules(DynamicArray<IdEnv> ids_minus, DynamicArray<IdEnv> ids_plus);

};

/* ************************************************************************ */

/**
 * @brief Parse reactions code.
 *
 * @param code       Reactions code.
 *
 * @return Result reactions.
 */
inline Reactions parseReactions(const String& code)
{
    return ReactionsParser(code).parse();
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
