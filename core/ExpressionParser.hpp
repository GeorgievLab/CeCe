#pragma once

#include "core/Range.hpp"
#include "core/Map.hpp"
#include "core/String.hpp"
#include "core/constants.hpp"

float parseExpression(IteratorRange<const char*>& range, const Map<String, float>& parameters);

inline float parseExpression(const String& expression, const Map<String, float>& parameters)
{
    auto range = makeRange(expression.c_str(), expression.c_str() + expression.size());
    return parseExpression(range, parameters);
}

class ExpressionParserException: public std::exception{};

class EmptyExpressionException: public ExpressionParserException
{
    virtual const char* what() const throw()
    {
        return "Cannnot parse empty expression.";
    }
};

class MissingParenthesisException: public ExpressionParserException
{
    virtual const char* what() const throw()
    {
        return "Missing closing parethesis.";
    }
};

class UnknownConstantException: public ExpressionParserException
{
    virtual const char* what() const throw()
    {
        return "Unknown constant name.";
    }
};

class UnknownFunctionException: public ExpressionParserException
{
    virtual const char* what() const throw()
    {
        return "Unknown function name.";
    }
};
