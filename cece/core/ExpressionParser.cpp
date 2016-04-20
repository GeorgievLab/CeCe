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
#include "cece/core/ExpressionParser.hpp"

// C++
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>

// CeCe
#include "cece/core/constants.hpp"
#include "cece/core/Tokenizer.hpp"
#include "cece/core/UnitIo.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Expression token codes.
 */
enum class ExpressionTokenCode
{
    Invalid,
    Identifier,
    Number,
    Plus,
    Minus,
    Multiply,
    Divide,
    Power,
    ParenOpen,
    ParenClose
};

/* ************************************************************************ */

/**
 * @brief Expression tokenizer.
 */
class ExpressionTokenizer
    : public BasicTokenizer<
        ExpressionTokenizer,
        BasicToken<ExpressionTokenCode, String>,
        const char*
    >
{

// Public Types
public:


    // Parent type
    using ParentType = BasicTokenizer<
        ExpressionTokenizer,
        BasicToken<ExpressionTokenCode, String>,
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
        TokenType token{ExpressionTokenCode::Number};

        while (isDigit() || is('.'))
        {
            token.value.push_back(value());
            next();
        }

        if (is('e'))
        {
            if (is('-'))
            {
                token.value.push_back(value());
                next();
            }

            while (isDigit() || is('.'))
            {
                token.value.push_back(value());
                next();
            }
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
        TokenType token{ExpressionTokenCode::Identifier};

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

        if (isDigit())
            return tokenizeNumber();

        if (isIdentifierBegin())
            return tokenizeIdentifier();

        switch (value())
        {
        case '+':
            return TokenType{ExpressionTokenCode::Plus, "+"};

        case '-':
            return TokenType{ExpressionTokenCode::Minus, "-"};

        case '*':
            return TokenType{ExpressionTokenCode::Multiply, "*"};

        case '/':
            return TokenType{ExpressionTokenCode::Divide, "/"};

        case '^':
            return TokenType{ExpressionTokenCode::Power, "^"};

        case '(':
            return TokenType{ExpressionTokenCode::ParenOpen, "("};

        case ')':
            return TokenType{ExpressionTokenCode::ParenClose, ")"};
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

}

/******************************************************************************************************/

class ExpressionParser
{

private:

    const String operators = "+-*/^();<> \n\r\t\v\b:";
    const String whitespace = " \n\r\t\v\b";
    const Parameters& parameters;
    IteratorRange<const char*>& iterator;

    void skipWhitespace()
    {
        while(!iterator.isEmpty() && std::find(std::begin(whitespace), std::end(whitespace), iterator.front()) != std::end(whitespace))
        {
            iterator.advanceBegin();
        }
    }

    String readConstant()
    {
        String local;
        while(!iterator.isEmpty() && std::find(std::begin(operators), std::end(operators), iterator.front()) == std::end(operators))
        {
            local += iterator.front();
            iterator.advanceBegin();
        }
        return local;
    }

    float signum(float source)
    {
        return (0 < source) - (source < 0);
    }

    float add()
    {
        float value = multiply();
        while (!iterator.isEmpty())
        {
            if (iterator.front() == '+')
            {
                iterator.advanceBegin();
                skipWhitespace();
                value = value + multiply();
            }
            else if(iterator.front() == '-')
            {
                iterator.advanceBegin();
                skipWhitespace();
                value = value - multiply();
            }
            else
                return value;
        }

        return value;
    }

    float multiply()
    {
        float value = power();
        while (!iterator.isEmpty())
        {
            if (iterator.front() == '*')
            {
                iterator.advanceBegin();
                skipWhitespace();
                value = value * power();
            }
            else if(iterator.front() == '/')
            {
                iterator.advanceBegin();
                skipWhitespace();
                value = value / power();
            }
            else
                return value;
        }

        return value;
    }

    float power()
    {
        float value = parenthesis();
        while (!iterator.isEmpty())
        {
            if (iterator.front() == '^')
            {
                iterator.advanceBegin();
                skipWhitespace();
                float exp = parenthesis();
                if (value == 0 && exp == 0)
                    return NAN;
                else
                    value = std::pow(value, exp);
            }
            else
                return value;
        }

        return value;
    }

    float parenthesis()
    {
        if (iterator.front() == '(')
        {
            iterator.advanceBegin();
            skipWhitespace();
            float value = add();
            if (iterator.front() == ')')
            {
                iterator.advanceBegin();
                skipWhitespace();
                return value;
            }
            else
                throw MissingParenthesisException();
        }
        else
            return constant();
    }

    float constant()
    {
        char* end;
        float value = strtof(iterator.begin(), &end);
        if (iterator.begin() != end)
        {
            iterator = makeRange<const char*>(end, iterator.end());
            skipWhitespace();
            return value;
        }

        float coeff = 1;

        // Unary minus
        if (iterator.front() == '-')
        {
            coeff = -1;
            iterator.advanceBegin();
        }

        String local = readConstant();
        if (local == "pi" || local == "PI")
        {
            skipWhitespace();
            return coeff * constants::PI;
        }
        if (local == "e" || local == "E")
        {
            skipWhitespace();
            return coeff * constants::E;
        }

        if (!parameters.exists(local))
        {
            skipWhitespace();
            return coeff * function(local);
        }
        skipWhitespace();
        return coeff * units::parse(parameters.get(local));
    }

    float function(String local)
    {
        if (iterator.front() != '(')
            throw UnknownConstantException();

        if (local == "Sin" || local == "sin")
            return std::sin(parenthesis());
        else if (local == "Cos" || local == "cos")
            return std::cos(parenthesis());
        else if (local == "Tan" || local == "tan")
            return std::tan(parenthesis());
        else if (local == "Asin" || local == "asin")
            return std::asin(parenthesis());
        else if (local == "Acos" || local == "acos")
            return std::acos(parenthesis());
        else if (local == "Atan" || local == "atan")
            return std::atan(parenthesis());
        else if (local == "Sinh" || local == "sinh")
            return std::sinh(parenthesis());
        else if (local == "Cosh" || local == "cosh")
            return std::cosh(parenthesis());
        else if (local == "Tanh" || local == "tanh")
            return std::tanh(parenthesis());
        else if (local == "Sqrt" || local == "sqrt")
            return std::sqrt(parenthesis());
        else if (local == "Log" || local == "log")
            return std::log10(parenthesis());
        else if (local == "Ln" || local == "ln")
            return std::log(parenthesis());
        else if (local == "Sgn" || local == "sgn")
            return signum(parenthesis());
        else if (local == "Abs" || local == "abs")
            return std::abs(parenthesis());
        else
            throw UnknownFunctionException();
    }

public:

    ExpressionParser(IteratorRange<const char*>& range, const Parameters& param) noexcept
        : parameters(param), iterator(range)
    {
        // Nothing to do
    }

    float parse()
    {
        skipWhitespace();
        if (iterator.isEmpty())
            throw EmptyExpressionException();
        return add();
    }
};

/* ************************************************************************ */

float parseExpressionRef(IteratorRange<const char*>& range, const Parameters& parameters)
{
    return ExpressionParser(range, parameters).parse();
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
