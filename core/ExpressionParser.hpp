#pragma once

#include "core/Range.hpp"

float parseExpression(IteratorRange<const char*>& range, const Map<String, float>& parameters);

inline float parseExpression(const String& expression, const Map<String, float>& parameters)
{
    auto range = makeRange(expression.c_str(), expression.c_str() + expression.size());
    return parseExpression(range, parameters);
}