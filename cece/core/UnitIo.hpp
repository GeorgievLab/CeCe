/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/String.hpp"
#include "cece/core/StringView.hpp"
#include "cece/core/InStream.hpp"
#include "cece/core/OutStream.hpp"
#include "cece/core/Unit.hpp"
#include "cece/core/UnitSymbol.hpp"

/* ************************************************************************ */

namespace cece {
inline namespace core {

/* ************************************************************************ */

namespace units {

/* ************************************************************************ */

/**
 * @brief Parse units value.
 *
 * This function can handle unit suffix and convert it into proper coefficient.
 *
 * @note Unit prefix is not supported because there is issue with meters:
 * `mg` - it's milligram or metergram?
 *
 * @param is Input stream.
 *
 * @return Result value.
 */
Value parse(InStream& is);

/* ************************************************************************ */

/**
 * @brief Parse units value.
 *
 * This function can handle unit suffix and convert it into proper coefficient.
 *
 * @note Unit prefix is not supported because there is issue with meters:
 * `mg` - it's milligram or metergram?
 *
 * @param value Value to parse.
 *
 * @return Result value.
 */
Value parse(StringView value);

/* ************************************************************************ */

/**
 * @brief Input stream operator.
 *
 * @param is   Input stream.
 * @param unit Result value.
 *
 * @return is.
 */
template<typename... Nominators, typename... Denominators>
InStream& operator>>(InStream& is, Unit<List<Nominators...>, List<Denominators...>>& unit)
{
    using Type = Unit<List<Nominators...>, List<Denominators...>>;
    using SymbolType = Symbol<Type>;

    // Type symbol
#if _LIBCPP_VERSION
    // MACOSX use old stdlib that doesnt support constexpr std::array.
    static const auto typeSymbolNom   = SymbolType::nominators::get();
    static const auto typeSymbolDenom = SymbolType::denominators::get();
#else
    static constexpr auto typeSymbolNom   = SymbolType::nominators::get();
    static constexpr auto typeSymbolDenom = SymbolType::denominators::get();
#endif

    Value val;
    String symbol;

    is >> std::ws >> val;

    // Unable to load unit
    if (!is)
        return is;

    // No symbol given
    if (!(is >> std::noskipws >> symbol))
    {
        is.clear();

        // Set value
        unit = Type{val};
        return is;
    }

    // Split symbol to two parts
    const auto sepPos = symbol.find('/');

    String symbolNom;
    String symbolDenom;

    // Only nominators
    if (sepPos == String::npos)
    {
        symbolNom = symbol;
    }
    else
    {
        symbolNom = symbol.substr(0, sepPos);
        symbolDenom = symbol.substr(sepPos + 1);
    }

    // Get coefficient exponent
    const int exponent = 0
        // Base given by type
        + Type::exponent
        // Nominators
        + calcPrefixExponent(symbolNom, typeSymbolNom, Type::firstCountNom)
        // Denominators
        - calcPrefixExponent(symbolDenom, typeSymbolDenom, Type::firstCountDenom)
    ;

    // Value coefficient
    const Value coefficient = exponentToCoefficient(exponent);

    // Set unit
    unit = Type(val * coefficient);

    return is;
}

/* ************************************************************************ */

/**
 * @brief Output stream operator.
 *
 * @param os   Output stream.
 * @param unit Input value.
 *
 * @return os.
 */
template<typename... Nominators, typename... Denominators>
OutStream& operator<<(OutStream& os, const Unit<List<Nominators...>, List<Denominators...>>& unit) noexcept
{
    os << unit.value();

    // TODO: write suffix

    return os;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
