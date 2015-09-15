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
#include "core/Units.hpp"

// Simulator
#include "core/Map.hpp"
#include "core/StringStream.hpp"

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

#if 0
template class Unit<List<BaseLength>, List<>>;
template class Unit<List<BaseMass>, List<>>;
template class Unit<List<BaseTime>, List<>>;
template class Unit<List<BaseLength, BaseLength>, List<>>;
template class Unit<List<BaseLength, BaseLength, BaseLength>, List<>>;
template class Unit<List<BaseLength>, List<BaseTime>>;
template class Unit<List<BaseLength>, List<BaseTime, BaseTime>>;
template class Unit<List<BaseLength, BaseMass>, List<BaseTime, BaseTime>>;
template class Unit<List<BaseMass>, List<BaseLength, BaseLength, BaseLength>>;
template class Unit<List<BaseMass>, List<BaseLength, BaseTime>>;
template class Unit<List<BaseLength, BaseLength>, List<BaseTime>>;
#endif

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

const Map<String, int> g_abbreviations = {
    // Molar (10e3 mol/m3)
    {"M", 3}
};

/* ************************************************************************ */

}

/* ************************************************************************ */

int calcPrefixExponent(char c, unsigned int count)
{
    if (count == 0)
        throw InvalidArgumentException("Unit symbol prefix doesn't make sence without nominator");

    // Result exponent
    int exponent = 0;

    // Unit prefix
    switch (c)
    {
    default:
        throw InvalidArgumentException("Unknown/unsupported unit symbol prefix: " + String(1, c));

    case 'G':
        // Giga
        exponent += 3;
    case 'M':
        // Mega
        exponent += 3;
    case 'k':
        // Kilo
        exponent += 3;
        break;

    case 'n':
        // Nano
        exponent -= 3;
    case 'u':
        // Micro
        exponent -= 3;
    case 'm':
        // Milli
        exponent -= 1;
    case 'c':
        // Centi
        exponent -= 1;
    case 'd':
        // Deci
        exponent -= 1;
    }

    return exponent * count;
}

/* ************************************************************************ */

int calcPrefixExponent(const String& symbol, StringView typeSymbol, unsigned int count)
{
    if (symbol.empty())
        return 0;

    const auto pos = symbol.find(typeSymbol.getData());
    if (pos == String::npos)
    {
        // Try to find abbreviations without prefix
        const auto it1 = g_abbreviations.find(symbol);

        if (it1 != g_abbreviations.end())
            return it1->second;

        // Try to find abbreviations with prefix
        const auto it2 = g_abbreviations.find(symbol.substr(1));

        if (it2 != g_abbreviations.end())
            return calcPrefixExponent(symbol.front(), count) + it2->second;

        throw InvalidArgumentException(
            "Invalid units symbol. "
            "Expected '(prefix)" + String(typeSymbol) + "' "
            "given '" + symbol + "'"
        );
    }

    // Get unit prefix
    if (pos == 1)
    {
        return calcPrefixExponent(symbol.front(), count);
    }
    else if (pos != 0)
    {
        throw InvalidArgumentException("Unit symbol prefix is longer than expected");
    }

    return 0;
}

/* ************************************************************************ */

Value parse(InStream& is)
{
    static const Map<String, int> predefinedSymbols = {
        // Millimolar - mol/m3
        {"mM", AMOUNT_OF_SUBSTANCE_EXPONENT - 3 * LENGTH_EXPONENT},
        // Micromolar - 10e-3 mol/m3
        {"uM", AMOUNT_OF_SUBSTANCE_EXPONENT - 3 * LENGTH_EXPONENT - 3},
        // Nanomolar - 10e-6 mol/m3
        {"nM", AMOUNT_OF_SUBSTANCE_EXPONENT - 3 * LENGTH_EXPONENT - 6}
    };

    Value value;
    String symbol;

    is >> std::ws >> value;

    // Unable to load unit
    if (!is)
        throw InvalidArgumentException("Cannot parse unit value");

    // No symbol given
    if (!(is >> std::noskipws >> symbol))
    {
        is.clear();

        // Return value without symbol
        return value;
    }

    int exponent = 0;

    // Try to find predefined symbols
    const auto it = predefinedSymbols.find(symbol);

    if (it != predefinedSymbols.end())
    {
        exponent = it->second;
    }
    else
    {
        // Foreach characters
        for (auto it = symbol.begin(); it != symbol.end(); ++it)
        {
            switch (*it)
            {
            case 'g':
                exponent += MASS_EXPONENT;
                break;

            case 's':
                exponent += TIME_EXPONENT;
                break;

            case 'm':
                // This is problematic, because it can be 'm' or 'mol'
                if (*(it + 1) == 'o')
                {
                    // Skip other two characters
                    it += 2;

                    exponent += AMOUNT_OF_SUBSTANCE_EXPONENT;
                }
                else
                {
                    exponent += LENGTH_EXPONENT;
                }
                break;

            default:
                throw InvalidArgumentException("Unsupported unit character '" + String(1, *it) + "' in '" + symbol + "'");
            }
        }
    }

    return value * exponentToCoefficient(exponent);
}

/* ************************************************************************ */

Value parse(StringView value)
{
    InStringStream is(value.getData());
    return parse(is);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

}

/* ************************************************************************ */
