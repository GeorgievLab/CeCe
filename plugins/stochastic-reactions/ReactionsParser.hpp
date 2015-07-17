/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
/* ************************************************************************ */

#pragma once

#include "Reactions.hpp"
#include "core/DynamicArray.hpp"
#include "core/ExpressionParser.hpp"
#include "core/Log.hpp"
#include "core/Tokenizer.hpp"
#include "core/Parser.hpp"

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
    Greater,
    Semicolon
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
        while (isRange('\0', ' '))
            next();

        // Skip comments
        while (is('#'))
        {
            find('\n', '\r');

            while (isRange('\0', ' '))
                next();
        }

        if (isIdentifierBegin())
            return tokenizeIdentifier();

        switch (value())
        {
        case '>':
            return TokenType{TokenCode::Greater};

        case ';':
            return TokenType{TokenCode::Semicolon};
        }

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

class ReactionsParser
    : public BasicParser<
        ReactionsParser,
        Tokenizer
    >
{
protected:

    //IteratorRange<const char*> range;
    const String whitespace = " \n\r\t\v\b";
    bool validator;
    bool reversible;

    virtual void check_push(String& id, DynamicArray<String>& array) = 0;

    void skipComments();

    DynamicArray<String> parseList();

    float parseRate(const char end_char);

public:

    ReactionsParser(const String& code) NOEXCEPT
        : BasicParser(code.c_str(), code.c_str() + code.size())
    {
        // Nothing to do.
    }


    // NOTE: This is only an example, real code should be in derived class
    ReactionsImpl parse()
    {
        ReactionsImpl result;

        // Parse: "A > 0.3 > B;"

        // First token must be identifier
        if (!is(TokenCode::Identifier))
            fatalError<ParserException>("Identifier expected");

        // Store source molecule
        result.source = token().value;

        // Next token
        next();

        // > required
        require(TokenCode::Greater);

        // Parse rate expression
        // NOTE: this modify tokenizer source range
        result.rate = parseExpression(getTokenizer().getRange());

        // > required
        require(TokenCode::Greater);

        // Identifier for output molecule
        if (!is(TokenCode::Identifier))
            fatalError<ParserException>("Identifier expected");

        // Store product molecule
        result.product = token().value;

        // Expression end
        require(TokenCode::Semicolon);

        return result;
    }
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
