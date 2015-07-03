#include "ExpressionParser.hpp"
#include "core/Range.hpp"
#include "core/Log.hpp"

class ExpressionParser
{
    Map<String, float> parameters;
    IteratorRange<const char*> iterator;
    
    ExpressionParser(Map<String, float>& param, InteratorRange<const char*> range) NOEXCEPT
    : parameters(param), iterator(range)
    {
        // Nothing to do
    }
    
    float parse()
    {
        return 0;
    }
}

float parseExpression(const String& expression, const Map<String, float>& parameters)
{
    return ExpressionParser(parameters, makeRange(expression)).parse();
}