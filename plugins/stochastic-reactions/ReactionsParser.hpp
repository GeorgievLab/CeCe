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
    Greater,
    Less,
    Plus,
    Colon,
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
        case '<':
            return TokenType{TokenCode::Less};
        case ':':
            return TokenType{TokenCode::Colon};
        case '+':
            return TokenType{TokenCode::Plus};
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
            validator = true;
            reversible = false;
            auto conditions = parseConditions();
            auto ids_minus = parseList();
            RateType rate;
            RateType rateR;
            if (reversible)
            {
                rateR = parseRate(',');
                rate = parseRate('>');
            }
            else
            {
                rate = parseRate('>');
            }
            auto ids_plus = parseList();
            if (validator)
            {
                reactions.extend(ids_plus, ids_minus, rate);
                if (reversible)
                    reactions.extend(ids_minus, ids_plus, rateR);
            }
            for (unsigned int i = 0; i < conditions.size(); i++)
            {
                reactions.addCondition(conditions[i].first, conditions[i].second);
            }
        }
        return reactions;
    }

        protected:


    // Use parent's member functions
    using ParentType::is;
    using ParentType::next;
    using ParentType::match;
    using ParentType::fatalError;
    using ParentType::require;


    bool validator;
    bool reversible;

    DynamicArray<std::pair<String, int>> parseConditions()
    {
        return DynamicArray<String, unsigned int>();
    }

    RateType parseRate(TokenCode end)
    {
        if (!validator)
            return 0;
        if (!is(TokenCode::Invalid))
        {
            float rate;
            try
            {
                rate = parseExpression(range, {});
            }
            catch (const ExpressionParserException& ex)
            {
                validator = false;
                Log::warning(ex.what());
                return 0;
            }
            if (range.front() == end_char)
            {
                range.advanceBegin();
                return rate;
            }
            validator = false;
            Log::warning("Rate area is either invalid or has no end.");
            return 0;
        }
        validator = false;
        Log::warning("Reaction has no rate area.");
        return 0;
    }

    DynamicArray<String> parseList()
    {
        if (!validator)
            return DynamicArray<String> ();
        DynamicArray<String> array;
        while (!is(TokenCode::Invalid))
        {
            if (is(TokenCode::Identifier))
            {
                check_push(token().value, array);
                next();
            }
            else if (is(TokenCode::Greater))
            {
                next();
                return array;
            }
            else if (is(TokenCode::Less))
            {
                reversible = true;
                next();
                return array;
            }
            else if (is(TokenCode::Plus))
            {
                next();
            }
            else
            {
                break;
            }
        }
        validator = false;
        Log::warning("Left side  of reaction has no end.");
        return DynamicArray<String> ();
    }

    void check_push(String& id, DynamicArray<String>& array)
    {
        if (id.size() == 0)
        {
            validator = false;
            Log::warning("I cannot accept empty molecule name. You may want to use 'null' molecule in degradation/expression reactions.");
        }
        else if (id == "null")
        {
            id.clear();
        }
        else if (id == "environment")
        {
            array.push_back("env");
            id.clear();
        }
        else
        {
            array.push_back(id);
            id.clear();
        }
    }

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
