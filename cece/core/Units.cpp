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
#include "cece/core/Units.hpp"

// CeCe
#include "cece/core/Map.hpp"
#include "cece/core/StringStream.hpp"
#include "cece/core/UnitsCtors.hpp"

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

/**
 * @brief Check if given character can be a part of symbol.
 *
 * @param c
 *
 * @return
 */
bool isSymbolChar(char c) noexcept
{
    return (
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') ||
        (c == '/')
    );
}

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
    String str;
    is >> str;

    return parse(str);
}

/* ************************************************************************ */

Value parse(StringView str)
{
    char* fSymbol;

    // Read float value
    const Value value = std::strtof(str.getData(), &fSymbol);

    // Cannot be read
    if (fSymbol == str.getData())
        throw InvalidArgumentException("Cannot parse unit value: " + String(str));

    String symbol;
    const char* eSymbol = str.getData() + str.getLength();

    // Store symbol characters
    for (; fSymbol != eSymbol && isSymbolChar(*fSymbol); ++fSymbol)
        symbol.push_back(*fSymbol);

    // No symbol
    if (symbol.empty())
        return value;

    // Predefined symbols
#define SYMBOL_TEST(nom) if (symbol == #nom) return nom(value).value()
#define SYMBOL_TEST2(nom, denom) if (symbol == #nom "/" #denom) return nom ## _ ## denom(value).value()

    SYMBOL_TEST(m);
    SYMBOL_TEST(dm);
    SYMBOL_TEST(mm);
    SYMBOL_TEST(um);
    SYMBOL_TEST(g);
    SYMBOL_TEST(kg);
    SYMBOL_TEST(mg);
    SYMBOL_TEST(ug);
    SYMBOL_TEST(ng);
    SYMBOL_TEST(pg);
    SYMBOL_TEST(s);
    SYMBOL_TEST(ms);
    SYMBOL_TEST(us);
    SYMBOL_TEST(min);
    SYMBOL_TEST(h);
    SYMBOL_TEST(m2);
    SYMBOL_TEST(dm2);
    SYMBOL_TEST(mm2);
    SYMBOL_TEST(um2);
    SYMBOL_TEST(m3);
    SYMBOL_TEST(dm3);
    SYMBOL_TEST(mm3);
    SYMBOL_TEST(um3);
    SYMBOL_TEST2(m, s);
    SYMBOL_TEST2(mm, s);
    SYMBOL_TEST2(um, s);
    SYMBOL_TEST2(m, s2);
    SYMBOL_TEST2(mm, s2);
    SYMBOL_TEST2(um, s2);
    SYMBOL_TEST2(kgm, s2);
    SYMBOL_TEST2(gm, s2);
    SYMBOL_TEST2(mgm, s2);
    SYMBOL_TEST(N);
    SYMBOL_TEST(mN);
    SYMBOL_TEST(uN);
    SYMBOL_TEST2(Ns, m2);
    SYMBOL_TEST(Pas);
    SYMBOL_TEST(mPas);
    SYMBOL_TEST2(m2, s);
    SYMBOL_TEST2(mm2, s);
    SYMBOL_TEST2(um2, s);
    SYMBOL_TEST(mol);
    SYMBOL_TEST(mmol);
    SYMBOL_TEST(umol);
    SYMBOL_TEST(nmol);
    SYMBOL_TEST2(mol, m3);
    SYMBOL_TEST2(mmol, m3);
    SYMBOL_TEST2(umol, m3);
    SYMBOL_TEST2(nmol, m3);
    SYMBOL_TEST(M);
    SYMBOL_TEST(mM);
    SYMBOL_TEST(uM);
    SYMBOL_TEST(nM);
    SYMBOL_TEST2(mol, um3);
    SYMBOL_TEST2(mmol, um3);
    SYMBOL_TEST2(umol, um3);
    SYMBOL_TEST2(nmol, um3);

#undef SYMBOL_TEST
#undef SYMBOL_TEST2

    // Fallback

    int exponent = 0;

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

    return value * exponentToCoefficient(exponent);
}

/* ************************************************************************ */

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
