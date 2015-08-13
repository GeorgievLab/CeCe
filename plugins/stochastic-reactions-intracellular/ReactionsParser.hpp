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
#include "core/Tuple.hpp"

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
    Not
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

/* ************************************************************************ */
    /**
     * @brief Constructor.
     *
     * @param code       Source code to parse.
     * @param parameters Reactions parameters.
     */
    explicit ReactionsParser(const String& code, const Map<String, float>& parameters = Map<String, float>{}) noexcept
        : ParentType(code.c_str(), code.c_str() + code.size())
        , m_parameters(parameters)
    {
        // Nothing to do.
    }

/* ************************************************************************ */
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

/* ************************************************************************ */
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
            if (reversible)
                rateR = parseRateReversible();
            rate = parseRate();
            auto ids_plus = parseList();
            requireNext(TokenCode::Semicolon);
            reactions.extend(ids_plus, ids_minus, rate);
            if (reversible)
                reactions.extend(ids_minus, ids_plus, rateR);
            auto no_cond = reactions.m_rules[reactions.m_rules.size() - 1];
            for (unsigned int i = 0; i < conditions.size(); i++)
            {
                reactions.addCondition(std::get<0>(conditions[i]), std::get<1>(conditions[i]), reversible, std::get<2>(conditions[i]), no_cond);
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

/* ************************************************************************ */

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
    /**
     * @brief Parse conditions for conditional reactions.
     *
     * @return tuple - conditions
     */
    DynamicArray<Tuple<String, unsigned int, bool>> parseConditions()
    {
        DynamicArray<Tuple<String, unsigned int, bool>> array;
        if (is(TokenCode::If))
        {
            bool clone = false;
            do
            {
                do
                {
                    next();
                    String name;
                    unsigned int requirement = 1;
                    if (is(TokenCode::Not))
                    {
                        requirement = 0;
                        next();
                    }
                    if (is(TokenCode::Identifier))
                    {
                        name = token().value;
                        next();
                    }
                    else
                        throw MissingIdentifierException{};
                    array.push_back(std::make_tuple(name, requirement, clone));
                    clone = false;
                }
                while (is(TokenCode::And));
                clone = true;
            }
            while (is(TokenCode::Or));

            requireNext(TokenCode::Colon);
            return array;
        }
        return array;
    }

/* ************************************************************************ */
    /**
     * @brief Parse rate expression for reversible reaction.
     *
     * @return
     */
    RateType parseRateReversible()
    {
        // Alias to tokenizer range.
        const auto& tokenizerRange = getTokenizer().getRange();

        // Store rate begin
        auto begin = tokenizerRange.begin();

        next();
        find(TokenCode::Comma, TokenCode::Semicolon);

        if (!is(TokenCode::Comma))
            throw MissingArrowException();

        auto end = tokenizerRange.begin() - 1;
        auto rate = parseExpression(makeRange(begin, end), m_parameters);
        //next();
        return rate;
    }

/* ************************************************************************ */
    /**
     * @brief Parse rate expression.
     *
     * @return
     */
    RateType parseRate()
    {
        // Alias to tokenizer range.
        const auto& tokenizerRange = getTokenizer().getRange();

        // Store rate begin
        auto begin = tokenizerRange.begin();

        // Last token was the one before rate - ArrowFwrd
        next();
        // Find arrow or semicolon
        find(TokenCode::ArrowFwrd, TokenCode::Semicolon);

        if (!is(TokenCode::ArrowFwrd))
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
template<typename ReactionsType>
inline ReactionsType parseReactions(const String& code, const Map<String, float>& parameters = Map<String, float>{})
{
    return ReactionsParser<ReactionsType>(code, parameters).parse();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
