/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Václav Pelíšek <pelisekv@students.zcu.cz>                        */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

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
    ArrowFwrd,
    ArrowBack,
    Plus,
    Colon,
    Semicolon,
    Comma
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

        // Full forward arrow string.
        // Starts at '>'
        const char* arrow_fwd_str = "->" + 1;

        switch (value())
        {
        case '-':
            next();
            if (!is('>'))
                fatalError("Invalid token");

            // Reset to full arrow string
            --arrow_fwd_str;

        case '>':
            next();
            return TokenType{TokenCode::ArrowFwrd, arrow_fwd_str};

        case '<':
            next();
            match('-');

            return TokenType{TokenCode::ArrowBack};

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

template<typename T>
class ReactionsParser
    : public BasicParser<
        ReactionsParser<T>,
        Tokenizer
    >
{

// Public Types
public:

    // Parent type.
    using ParentType = BasicParser<
        ReactionsParser<T>,
        Tokenizer
    >;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param code Source code to parse.
     */
    explicit ReactionsParser(const String& code) noexcept
        : ParentType(code.c_str(), code.c_str() + code.size())
    {
        // Nothing to do.
    }


    /**
     * @brief Parse source string into reactions.
     *
     * @return
     */
    T parse()
    {
        T reactions;
        while (!is(TokenCode::Invalid))
        {
            parseReaction(reactions);
        }
        return reactions;
    }

    void parseReaction(T& reactions)
    {
        try
        {
            auto conditions = parseConditions();
            auto ids_minus = parseList();
            if (!is(TokenCode::ArrowBack, TokenCode::ArrowFwrd))
                throw MissingArrowException();
            RateType rate;
            RateType rateR;
            bool reversible = is(TokenCode::ArrowBack);
            next();
            if (reversible)
                rateR = parseRateReversible();
            rate = parseRate();
            auto ids_plus = parseList();
            requireNext(TokenCode::Semicolon);
            reactions.extend(ids_plus, ids_minus, rate);
            if (reversible)
                reactions.extend(ids_minus, ids_plus, rateR);
            for (unsigned int i = 0; i < conditions.size(); i++)
            {
                reactions.addCondition(conditions[i].first, conditions[i].second);
            }
        }
        catch (const Exception& ex)
        {
            Log::warning(ex.what());
            find(TokenCode::Semicolon);
            next();
            return;
        }
    }


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

    DynamicArray<std::pair<String, unsigned int>> parseConditions()
    {
        return DynamicArray<std::pair<String, unsigned int>>();
    }

/* ************************************************************************ */

    RateType parseRateReversible()
    {
        const char* begin = getTokenizer().getRange().begin();
        find(TokenCode::Comma);
        if (is(TokenCode::Invalid))
            throw MissingArrowException();
        const char* end = getTokenizer().getRange().begin() - 1;
        auto range = makeRange(begin, end);
        auto rate = parseExpression(range);
        next();
        return rate;
    }

/* ************************************************************************ */

    RateType parseRate()
    {
        const char* begin = getTokenizer().getRange().begin();
        find(TokenCode::ArrowFwrd);
        if (is(TokenCode::Invalid))
            throw MissingArrowException();
        const char* end = getTokenizer().getRange().begin() - token().value.size();
        auto range = makeRange(begin, end);
        auto rate = parseExpression(range);
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

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
