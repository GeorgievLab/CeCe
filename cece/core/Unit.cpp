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
#include "cece/core/Unit.hpp"

// CeCe
#include "cece/core/Map.hpp"
#include "cece/core/Exception.hpp"

/* ************************************************************************ */

namespace cece {
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

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
