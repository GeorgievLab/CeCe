/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                                */
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

// GTest
#include <gtest/gtest.h>

// C++
#include <cstring>

// CeCe
#include "cece/core/Assert.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Tokenizer.hpp"

/* ************************************************************************ */

using namespace cece;

/* ************************************************************************ */

/**
 * @brief Test token codes.
 */
enum class TestTokenCode
{
    Invalid,
    Identifier,
    Number,
    Operator,
};

/* ************************************************************************ */

/**
 * @brief Test tokenizer.
 */
class TestTokenizer
    : public BasicTokenizer<
        TestTokenizer,
        BasicToken<TestTokenCode, String>,
        const char*
    >
{

// Public Types
public:


    // Parent type
    using ParentType = BasicTokenizer<
        TestTokenizer,
        BasicToken<TestTokenCode, String>,
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
     * @brief Tokenize number.
     *
     * @return
     */
    TokenType tokenizeNumber() noexcept
    {
        TokenType token{TestTokenCode::Number};

        while (isDigit() || is('.'))
        {
            token.value.push_back(value());
            next();
        }

        // Suffix
        if (is('f'))
        {
            token.value.push_back(value());
            next();
        }

        return token;
    }


    /**
     * @brief Tokenize identifier.
     *
     * @return
     */
    TokenType tokenizeIdentifier() noexcept
    {
        TokenType token{TestTokenCode::Identifier};

        Assert(isIdentifierBegin());

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
     * @return Tokenized token.
     */
    TokenType tokenize() noexcept
    {
        // Skip whitespaces
        while (value() <= 0x20)
            next();

        if (isEof())
            return {};

        if (isDigit())
            return tokenizeNumber();

        if (isIdentifierBegin())
            return tokenizeIdentifier();

        switch (value())
        {
        case '+':
        case '-':
        case '*':
        case '/':
        {
            auto val = value();
            next();
            return TokenType{TestTokenCode::Operator, String(1, val)};
        }
        }

        return {};
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

TEST(Tokenizer, expression)
{
    {
        const char* str = "10 13.f + - * hello im_2";

        TestTokenizer tokenizer(str, str + strlen(str));

        auto it = tokenizer.begin();
        ASSERT_EQ(TestTokenCode::Number, *it);
        ASSERT_EQ("10", *it);

        ++it;
        ASSERT_EQ(TestTokenCode::Number, *it);
        ASSERT_EQ("13.f", *it);

        ++it;
        ASSERT_EQ(TestTokenCode::Operator, *it);
        ASSERT_EQ("+", *it);

        ++it;
        ASSERT_EQ(TestTokenCode::Operator, *it);
        ASSERT_EQ("-", *it);

        ++it;
        ASSERT_EQ(TestTokenCode::Operator, *it);
        ASSERT_EQ("*", *it);

        ++it;
        ASSERT_EQ(TestTokenCode::Identifier, *it);
        ASSERT_EQ("hello", *it);

        ++it;
        ASSERT_EQ(TestTokenCode::Identifier, *it);
        ASSERT_EQ("im_2", *it);

        ++it;
        ASSERT_EQ(tokenizer.end(), it);
    }
}

/* ************************************************************************ */
